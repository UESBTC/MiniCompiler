#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 100

void E();
void E1();
void T();
void T1();
void F();
void error();
void advance();
void queInit();
char outQue();
void outputCur();

char symQue[MAX_LEN];
int top = 0;
int bottom=0;

char sym;
FILE *inFile;

void queInit() {
    advance();
    symQue[top++]=sym;
    while(sym != EOF) {
        advance();
        symQue[top++]=sym;
    }
    outQue();
}

char outQue() {
    if(top != bottom){
        return symQue[++bottom];
    }
}

void outputCur() {
    printf("\n%d\n", bottom);

    printf("已分析过的串：");
    for(int i=0; i<bottom; i++){
        printf("%c", symQue[i]);
    }
    printf("\n");

    printf("当前字符：%c\n", sym);

    printf("剩余串：");
    for(int i=bottom; i<top; i++){
        printf("%c", symQue[i]);
    }
    printf("\n");
}

int main(int argc,const char * argv[])  {
    inFile=fopen(argv[1],"r");
    queInit();
    if (inFile==NULL){
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    sym=outQue();
    E();

}


void advance()  {
    sym=getc(inFile);
    if (sym == ' ' || sym == '\t'){
        while(sym == ' ' || sym == '\t')
        sym = getc(inFile);
    }
}

void E() {
    printf("=================================================\n");
    printf("使用的产生式：E --> TE\'\n");
    outputCur();
    printf("=================================================\n");
    T();
    E1();
}

void E1() {
    if(sym == '+') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> +TE\'\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
        T();
        E1();
    }
    else if(sym == '-') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> -TE\'\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
        T();
        E1();
    }
}

void T() {
    printf("=================================================\n");
    printf("使用的产生式：T --> FT\'\n");
    outputCur();
    printf("=================================================\n");
    F();
    T1();
}

void T1() {
    if(sym == '*') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
        F();
        T1();
    }
    else if(sym == '/') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
        F();
        T1();
    }
}

void F() {
    if(sym == '(') {
        printf("=================================================\n");
        printf("使用的产生式：F --> (E)\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
        E();
        if(sym == ')') {
            sym=outQue();
        }
        else {
            error();
        }
    }
    else if(sym == 'i') {
        printf("=================================================\n");
        printf("使用的产生式：F --> i\n");
        outputCur();
        printf("=================================================\n");
        sym=outQue();
    }
    else{
        error();
    }
}

void error(){
    printf("ERROR\n");
    return;
}