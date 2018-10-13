//
//  main.c
//  WordAnalysier
//
//  Created by 陈泽宁 on 2018/10/12.
//  Copyright © 2018 陈泽宁. All rights reserved.
//
/*2018-10-12
 主要修改之处：将 getc() 改成了自定义的 getnbc() [并没有什么用] 见33、56、57、63行
 将 空格的判断机制函数 getNonBlank() 修改成识别 空格 与 制表符，从而捕获 换行符 见80行
 */
/*2018-10-13
 行数安排上了
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <String.h>
int line=1;
int charClass;
#define MAX_LEN 100
char lexeme[MAX_LEN];
char nextChar;
char next2Char;
int lexLen;
int token;
int nextToken;
FILE *inFile;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 999
enum{WHILE=258,IF,ELSE,FOR,RET,INT,ID,EQU,GEQU,LEQU,ERROR};
char *keywords[]={"while","if","else","for","return",0};

char getnbc()   {
    char ch = fgetc(inFile);
    while (ch == ' ' || ch == '\t') ch = fgetc(inFile);    //略过空格和制表符
    return ch;
}

/*addChar()
 存储nextChar
 */
void addChar()  {
    if(lexLen<=MAX_LEN-2)   {
        lexeme[lexLen++]=nextChar;
        lexeme[lexLen]=0;
    }
    else
        printf("ERROR:lexme is too long. \n");
}
/*
 getChar()
 第一次运行：读入两个字符
 其他：nextChar next2Char后挪一位 判断nextChar的charClass并存储
 */
void getChar()  {
    static int firstRun=1;
    if(firstRun)    {
        nextChar=getnbc();
        next2Char=getnbc();
        firstRun=0;
    }   else    {
        nextChar=next2Char;
        next2Char=getnbc();
    }
    
    if(nextChar==EOF)   {
        charClass=EOF;
    }   else    {
        if(isalpha(nextChar))
            charClass=LETTER;
        else if(isdigit(nextChar))
            charClass=DIGIT;
        else
            charClass=UNKNOWN;
    }
}
void getNonBlank()  {
    while (nextChar==' '|| nextChar=='\t') {
        getChar();
    }
}

int checkSymbol(char ch, char nextCh)   {
    switch(ch)  {
        case'(':case')':case';':case'+':case'-':
            addChar();
            nextToken=ch;
            break;
        case'=':
            addChar();
            nextToken=ch;
            if(nextCh == '=')   {
                getChar();
                addChar();
                nextToken=EQU;
            }
            break;
        case'>':
            addChar();
            nextToken=ch;
            if(nextCh == '=')   {
                getChar();
                addChar();
                nextToken=GEQU;
            }
            break;
        case'<':
            addChar();
            nextToken=ch;
            if(nextCh == '=')   {
                getChar();
                addChar();
                nextToken=LEQU;
            }
            break;
        case '\n':
            addChar();
            line++;
            nextToken=27;
            break;
        case EOF:
            addChar();
            nextToken=EOF;
        default:
            printf("ERROR:unknown character '%c'.\n",ch);
            nextToken=ERROR;
    }
    return nextToken;
}

void checkKeywords(char* pword) {
    int i = 0;
    while(keywords[i] != 0) {
        char* pkeyword = keywords[i];
        if(strcmp(pword,pkeyword) == 0) {
            nextToken = 258 + i;
            return;
        }
        i++;
    }
}

int lexer() {
    lexLen = 0;
    getNonBlank();
    switch(charClass)   {
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT)    {
                addChar();
                getChar();
            }
            nextToken = ID;
            
            //检查当前标识符是否是关键字
            checkKeywords(lexeme);
            break;
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT)   {
                addChar();
                getChar();
            }
            nextToken = INT;
            break;
            
            
        case UNKNOWN:
            checkSymbol(nextChar,next2Char);
            getChar();
            break;
        case EOF:
            nextToken = EOF;
            lexeme[1] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    
    printf("line %02d:(%03d,numbers) (class):%s\n",line,nextToken,lexeme);
    return nextToken;
}


void main(int argc,const char * argv[]) {
    if (argc<2) {
        printf("ERROR:input file name is needed.\n");
        exit(0);
    }
    inFile=fopen(argv[1], "r");
    if (inFile==NULL)   {
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    
    getChar();
    while (nextToken!=EOF)  {
        lexer();
    }
    
}
