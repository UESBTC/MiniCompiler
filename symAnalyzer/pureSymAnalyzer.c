#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void E();
void E1();
void T();
void T1();
void F();
void error();
void advance();

char sym;
FILE* inFile;

int main(int argc,const char * argv[])  {
    inFile=fopen(argv[1],"r");

    if (inFile==NULL){
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    advance();
    
    while (sym!=EOF){
        E();
    }
    
}

void advance() {
    sym = getc(inFile);
    if (sym == ' ' || sym == '\t'){
        while(sym == ' ' || sym == '\t')
            sym = getc(inFile);
    }
    printf("\n!!!%c!!!\n", sym);
}

void E() {
    printf("=================================================\n");
    printf("使用的产生式：E --> TE\'\n");
    //outputCur(p);
    printf("=================================================\n");
    T();
    E1();
}

void E1() {
    if(sym == '+') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> +TE\'\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
        T();
        E1();
    }
    else if(sym == '-') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> -TE\'\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
        T();
        E1();
    }
    else{
        printf("=================================================\n");
        printf("使用的产生式：E\' --> ε\n");
        //outputCur(p);
        printf("=================================================\n");
    }
}

void T() {
    printf("=================================================\n");
    printf("使用的产生式：T --> FT\'\n");
    //outputCur(p);
    printf("=================================================\n");
    F();
    T1();
}

void T1() {
    if(sym == '*') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
        F();
        T1();
    }
    else if(sym == '/') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
        F();
        T1();
    }
    else{
        printf("=================================================\n");
        printf("使用的产生式：T\' --> ε\n");
        //outputCur(p);
        printf("=================================================\n");
    }
}

void F() {
    if(sym == '(') {
        printf("=================================================\n");
        printf("使用的产生式：F --> (E)\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
        E();
        if(sym == ')') {
            advance();
        }
        else {
            error();
        }
    }
    else if(sym == 'i') {
        printf("=================================================\n");
        printf("使用的产生式：F --> i\n");
        //outputCur(p);
        printf("=================================================\n");
        advance();
    }
    else{
        error();
    }
}

void error(){
    printf("ERROR\n");
    exit(-1);
}