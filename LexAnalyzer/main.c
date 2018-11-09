#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int charClass;
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 999

#define MAX_LEN 1000
char lexeme[MAX_LEN];

char nextChar;
char next2Char;
int lexLen;
int nextToken;
int num[100]={0};
int line = 1;


FILE *inFile;
FILE *lex;
FILE *symtable;
FILE *error;
/*
 enum:根据符号表定义的所有类别
 */
enum{
    $SYMBOL=1,$CONSTANT,$INT,$IF,$ELSE,$WHILE,$FOR,$READ,$WRITE,
    $ADD,$SUB,$MUL,$DIV,$L,$LE,$G,$GE,$NE,$E,
    $ASSIGN,$LPAR,$RPAR,$COM,$SEM,$DOT,$COLON,$LB,$RB,$AP,$QM,$BS,
    $NEXTLINE,$AND,$OR,$SHARP,$PR,$NOT,$SA,$SO,
    $ERROR,$SWITCH,$CASE,$DEFAULT,$VOID,$RETURN,$CONTINUE,$BREAK,$COMMENT
};
/*
 关键字
 */
char *keywords[]={
    "int","if","else","while","for","read","write",0,
    "case","switch","case","default","void","return","continue","break"
};
/*
 通过类别编号返回类别，用于最终输出词法分析最终结果
 */
char * classcifier(int i){
    if(i==1)
        return "标识符";
    else if(i==2)
        return "常数";
    else if((i>=3&&i<=9) || (i>=40&&i<=47))
        return "关键字";
    else if((i>=10&&i<=19)|| (i>=33&&i<=39))
        return "运算符";
    else if(i>=20&&i<=31)
        return "界符";
    else if(i==32)
        return "换行符";
    else
        return "其他字符";
}

void addChar(){
    if(lexLen<=MAX_LEN-2){
        lexeme[lexLen++]=nextChar;
        lexeme[lexLen]=0;
    }
    else
        printf("ERROR:lexme is too long. \n");
}

void getChar(){
    static int firstRun=1;
    if(firstRun){
        nextChar=getc(inFile);
        next2Char=getc(inFile);
        firstRun=0;
    }
    else{
        nextChar=next2Char;
        next2Char=getc(inFile);
    }
    
    if(nextChar==EOF){
        charClass=EOF;
    }
    else{
        if(isalpha(nextChar))
            charClass=LETTER;
        else if(isdigit(nextChar))
            charClass=DIGIT;
        else
            charClass=UNKNOWN;
    }
}
/*
 在空白符中不能略去换行符，因为换行符需要识别
 */
void getNonBlank(){
    while (nextChar == ' ' || nextChar == '\t') {
        getChar();
    }
}
/*删注释*/
void SkipComment(void)  {
    getChar();
    addChar();
    while (nextChar!='\n') {
        getChar();
        addChar();
    }
}
int checkSymbol(char ch, char nextCh){
    switch(ch){
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
            if (nextCh=='/') {
                getChar();
                addChar();
                SkipComment();
                nextToken=$COMMENT;
            }
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
            if(nextCh == '='){
                getChar();
                addChar();
                nextToken=$E;
            }
            break;
        case'>':
            addChar();
            nextToken=$G;
            if(nextCh == '='){
                getChar();
                addChar();
                nextToken=$GE;
            }
            break;
        case'<':
            addChar();
            nextToken=$L;
            if(nextCh == '='){
                getChar();
                addChar();
                nextToken=$LE;
            }
            break;
        case'!':
            addChar();
            nextToken=$NOT;
            if(nextCh == '='){
                getChar();
                addChar();
                nextToken=$NE;
            }
            break;
        case'&':
            addChar();
            nextToken=$SA;
            if(nextCh == '&'){
                getChar();
                addChar();
                nextToken=$AND;
            }
            break;
        case'|':
            addChar();
            nextToken=$SO;
            if(nextCh == '|'){
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
            printf("error: line %2d 含有非法字符 :%c\n",line,nextChar);
            fprintf(error,"error: line %2d 含有非法字符 :%c\n",line,nextChar);
            nextToken=$ERROR;
    }
    if(nextToken!=EOF)
        num[nextToken]++;
    return nextToken;
}
/*
 关键字检测
 */
void checkKeywords(char* pword){
    int i = 0;
    while(keywords[i] != 0){
        char* pkeyword = keywords[i];
        if(strcmp(pword,pkeyword) == 0){
            if(i<7) {
                nextToken = 3 + i;
                return;
            }
            else    {//在符号表40后又添加了关键字，为不改变符号表前24位定义，故多做分支判断识别
                nextToken=40+i;
                return;
            }
        }
        i++;
    }
}

int lexer(){
    lexLen = 0;
    getNonBlank();
    switch(charClass){
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = $SYMBOL;
            checkKeywords(lexeme);
            break;
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT){
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
    
    if (nextToken==40||nextToken==48) {//错误处理已经在checkSymbol()内进行打印输出，若此处再次对编号为40（$ERROR）类别的单词进行输出，会造成重复识别的问题，故用空语句跳过ERROR类的输出
        ;
    }
    else if(nextToken==32)  {//不希望换行符在输出时产生换行效果，故用打印“\n”来代替真正的换行符，使输出界面更加简洁
        printf("line %02d:(%02d,%03d) %s: \\n \n",line-1,nextToken,num[nextToken],classcifier(nextToken));
        fprintf(lex, "line %02d:(%02d,%03d) %s: \\n \n",line-1,nextToken,num[nextToken],classcifier(nextToken));
        fprintf(symtable, "%02d\t%02d\t%s\n",nextToken,line-1,classcifier(nextToken));
    }
    else if(nextToken==EOF) {//文件末尾的打印输出
        printf("line %02d:(%02d,001) 文件末尾: %s\n",line,nextToken,lexeme);
        fprintf(lex, "line %02d:(%02d,001) 文件末尾: %s\n",line,nextToken,lexeme);
        fprintf(symtable, "%02d\t%02d\t文件末尾\n",nextToken,line);
    }
    else    {
        printf("line %02d:(%02d,%03d) %s: %s\n",line,nextToken,num[nextToken],classcifier(nextToken),lexeme);
        fprintf(lex, "line %02d:(%02d,%03d) %s: %s\n",line,nextToken,num[nextToken],classcifier(nextToken),lexeme);
        fprintf(symtable, "%02d\t%02d\t%s\n",nextToken,line,classcifier(nextToken));
    }
    
    return nextToken;
}

void main(int argc, const char * argv[]) {
    printf("陈泽宁-2017221002004\n");//学号姓名
    /*
     文件指针的定义
     */
    lex=fopen("lex.txt", "w");
    symtable =fopen("symtable.txt", "w");
    error=fopen("error.txt", "w");
    
    fprintf(symtable,"符号表\n编号\t行号\t名称\n");
    if (argc<2){
        printf("ERROR:input file name is needed.\n");
        exit(0);
    }
    inFile=fopen(argv[1], "r");
    if (inFile==NULL){
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    getChar();
    while (nextToken!=EOF){
        lexer();
    }
    
}

