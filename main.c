#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int line = 1;
int charClass;               //字符的类型
#define MAX_LEN 1000          //存储字符的数组的最大值
char lexeme[MAX_LEN];        //存储字符的数组
char nextChar;               //nextChar和next2Char配合，不断向后读取字符，（见getChar()函数）
char next2Char;
int lexLen;                  //目前读到的字符长度，如果超出MAX_LEN-2 (?),则提示过长(见addChar())
//int token;
int nextToken;
FILE *inFile;
FILE *lex;
FILE *systable;
FILE *error;
int num[39]={0};
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 999
//enum{WHILE=258,IF,ELSE,FOR,RET,INT,ID,EQU,GEQU,LEQU,ERROR};
enum{$SYMBOL=1,$CONSTANT,$INT,$IF,$ELSE,$WHILE,$FOR,$READ,$WRITE,$ADD,$SUB,$MUL,$DIV,$L,$LE,$G,$GE,$NE,$E,$ASSIGN,$LPAR,$RPAR,$COM,$SEM,$AND,$OR,$NEXTLINE,$SHARP,$COLON,$LB,$RB,$AP,$QM,$PR,$BS,$NOT,$SA,$SO,$DOT,$ERROR,$SWITCH,$CASE,$DEFAULT,$VOID,$RETURN,$CONTINUE,$BREAK};
char *keywords[]={"int","if","else","while","for","read","write","switch","case","switch","case","default","void","return","continue","break",0};

char * classcifier(int i)   {
    if(i==1)
        return "标识符";
    else if(i>=3&&i<=9)
        return "关键字";
    else if(i==2)
        return "常数";
    else if((i>=10&&i<=19)||i==26||i==35||i==38||i==39)
        return "运算符";
    else if(i==23||i==24||i==30)
        return "界符";
    else
        return "其他字符";
}
void addChar()
{
    if(lexLen<=MAX_LEN-2)
    {
        lexeme[lexLen++]=nextChar;
        lexeme[lexLen]=0;
    }
    else
        printf("ERROR:lexme is too long. \n");
}

void getChar()
{
    static int firstRun=1;
    if(firstRun)
    {
        nextChar=getc(inFile);
        next2Char=getc(inFile);
        firstRun=0;
    }
    else
    {
        nextChar=next2Char;
        next2Char=getc(inFile);
    }
    
    if(nextChar==EOF)
    {
        charClass=EOF;
    }
    else
    {
        if(isalpha(nextChar))
            charClass=LETTER;
        else if(isdigit(nextChar))
            charClass=DIGIT;
        else
            charClass=UNKNOWN;
    }
}
void getNonBlank()
{
    while (nextChar == ' ' || nextChar == '\t') { //修改空白符号的判断标准，避免isspace()吃掉换行符
        getChar();
    }
}

int checkSymbol(char ch, char nextCh)
{
    switch(ch)
    {
        case'+':
            addChar();
            nextToken=$ADD;
            break;
        case'-':
            addChar();
            nextToken=$SUB;
            break;
        case'*':
            addChar();
            nextToken=$MUL;
            break;
        case'/':
            addChar();
            nextToken=$DIV;
            break;
        case'(':
            addChar();
            nextToken=$LPAR;
            break;
        case')':
            addChar();
            nextToken=$RPAR;
            break;
        case'{':
            addChar();
            nextToken=$LB;
            break;
        case'}':
            addChar();
            nextToken=$RB;
            break;
        case'#':
            addChar();
            nextToken=$SHARP;
            break;
        case';':
            addChar();
            nextToken=$SEM;
            break;
        case',':
            addChar();
            nextToken=$COM;
            break;
        case'.':
            addChar();
            nextToken=$DOT;
            break;
        case'\'':
            addChar();
            nextToken=$AP;
            break;
        case'\"':
            addChar();
            nextToken=$QM;
            break;
        case'%':
            addChar();
            nextToken=$PR;
            break;
        case'\\':
            addChar();
            nextToken=$BS;
            break;
        case'=':
            addChar();
            nextToken=$ASSIGN;
            if(nextCh == '=')
            {
                getChar();
                addChar();
                nextToken=$E;
            }
            break;
        case'>':
            addChar();
            nextToken=$G;
            if(nextCh == '=')
            {
                getChar();
                addChar();
                nextToken=$GE;
            }
            break;
        case'<':
            addChar();
            nextToken=$L;
            if(nextCh == '=')
            {
                getChar();
                addChar();
                nextToken=$LE;
            }
            break;
        case'!':
            addChar();
            nextToken=$NOT;
            if(nextCh == '=')
            {
                getChar();
                addChar();
                nextToken=$NE;
            }
            break;
        case'&':
            addChar();
            nextToken=$SA;
            if(nextCh == '&')
            {
                getChar();
                addChar();
                nextToken=$AND;
            }
            break;
        case'|':
            addChar();
            nextToken=$SO;
            if(nextCh == '|')
            {
                getChar();
                addChar();
                nextToken=$OR;
            }
            break;
            
        case'\n':
            addChar();
            nextToken=$NEXTLINE;
            line++;
            break;
        case EOF:
            addChar();
            nextToken=EOF;
        default:
            addChar();
            printf("ERROR:Unknown Character:%c\n",nextChar);
            nextToken=$ERROR;
    }
    if(nextToken!=EOF)
        num[nextToken]++;
    return nextToken;
}

void checkKeywords(char* pword)
{
    int i = 0;
    while(keywords[i] != 0)
    {
        char* pkeyword = keywords[i];
        if(strcmp(pword,pkeyword) == 0)
        {   if(i<7) {
            nextToken = 3 + i;
            return;
        }   else    {
            nextToken=41+i;
            return;
        }
            
        }
        i++;
    }
}

int lexer()
{
    lexLen = 0;
    getNonBlank();
    switch(charClass)
    {
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT)
            {
                addChar();
                getChar();
            }
            nextToken = $SYMBOL;
            
            //检查当前标识符是否是关键字
            checkKeywords(lexeme);
            break;
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT)
            {
                addChar();
                getChar();
            }
            nextToken = $CONSTANT;
            break;
            
            
        case UNKNOWN:
            checkSymbol(nextChar,next2Char);
            getChar();
            break;
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    if(nextToken != 27&&nextToken!=EOF) {
        printf("line %02d:(%02d,%03d) %s: %s\n",line,nextToken,num[nextToken],classcifier(nextToken),lexeme);
        fprintf(lex, "line %02d:(%02d,%03d) %s: %s\n",line,nextToken,num[nextToken],classcifier(nextToken),lexeme);
    }
    else if(nextToken==27)  {
        printf("line %02d:(%02d,%03d) %s: \\n \n",line-1,nextToken,num[nextToken],classcifier(nextToken));
        fprintf(lex, "line %02d:(%02d,%03d) %s: \\n \n",line-1,nextToken,num[nextToken],classcifier(nextToken));
    }
    else    {
        printf("line %02d:(%02d,001) 文件末尾: %s\n",line,nextToken,lexeme);
        fprintf(lex, "line %02d:(%02d,001) 文件末尾: %s\n",line,nextToken,lexeme);
    }
    
    
    return nextToken;
}

void main(int argc, const char * argv[]) {
    lex=fopen("lex.txt", "w");
    systable =fopen("systable.txt", "w");
    error=fopen("error.txt", "w");
    if (argc<2)
    {
        printf("ERROR:input file name is needed.\n");
        exit(0);
    }
    inFile=fopen(argv[1], "r");
    if (inFile==NULL)
    {
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    
    getChar();
    while (nextToken!=EOF)
    {
        lexer();
    }
    
}
