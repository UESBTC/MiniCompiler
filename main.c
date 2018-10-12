//
//  main.c
//  WordAnalysier
//
//  Created by 陈泽宁 on 2018/10/12.
//  Copyright © 2018 陈泽宁. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
    while (isspace(nextChar)) {
        getChar();
    }
}












void main(int argc, const char * argv[]) {
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
