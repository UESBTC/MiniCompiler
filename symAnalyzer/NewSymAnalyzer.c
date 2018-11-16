#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LEN 100

void E(char* p);
void E1(char* p);
void T(char* p);
void T1(char* p);
void F(char* p);
void error();
void advance(char* p_sym);    //读取字符用的函数     //尝试用指针代替全局变量

void initQue(char* p_sym);    //队列初始化
void inQue(char* p_sym);      //入队
char outQue();                //出队
void showQue();               //显示队列

void outputCur(char* p_sym);
int count = 0;                //计数器，记录当前分析字符的索引
char symQue[MAX_LEN];
int head = 0;
int tail = 0;
FILE* inFile;

int main(int argc,const char * argv[]) {
    char sym;
    char* p_sym = &sym;
    inFile=fopen(argv[1],"r");
    
    if (inFile==NULL) {
        printf("ERROR:can not open file.\n");
        exit(0);
    }
    
    advance(p_sym);          //这3行用于把文件里的东西读到symQue里面
    initQue(p_sym);
    showQue();
    
    
    rewind(inFile);          //注意这里必须把inFile指针指回文件开头
    count = 0;
    advance(p_sym);
    
    //while(*p_sym != EOF) {
        E(p_sym);
    //}
    
    return 0;
}

void inQue(char* p_sym) {
    if(head < MAX_LEN){
        symQue[head++] = *p_sym;
    }
    else
        printf("Is full\n");
}

void initQue(char *p_sym) {
    while(*p_sym != EOF) {
        inQue(p_sym);
        advance(p_sym);
    }
}

void showQue() {
    for(int i=tail; i<head; i++){
        printf("%c ",symQue[i]);
    }
    printf("\n");
}



char outQue() {
    if(tail <= head){
        return symQue[tail++];
    }
    else{
        printf("Is empty\n");
    }
}

void outputCur(char* p_sym) {
    //显示已分析串
    //printf("\ncount: %d\n", count);
    printf("已分析串: ");
    for(int i=0; i<count; i++) {
        printf("%c ", symQue[i-1]);
    }
    printf("\n");
    //显示当前字符
    if(*p_sym == EOF)
        printf("当前字符: EOF\n");    
    else
        printf("当前字符: %c\n", *p_sym);

    //显示未分析串
    printf("剩余串: ");
    for(int i=count; i<head; i++) {
        printf("%c", symQue[i]);
    }
    printf("\n");
}

void advance(char* p_sym) {
    *p_sym = getc(inFile);
    if (*p_sym == ' ' || *p_sym == '\t'){
        while(*p_sym == ' ' || *p_sym == '\t')
            *p_sym = getc(inFile);
    }
    count++;
    //printf("\n!!!%c!!!\n", *p_sym);
}

void E(char* p) {
    printf("=================================================\n");
    printf("使用的产生式：E --> TE\'\n");
    outputCur(p);
    printf("=================================================\n");
    T(p);
    E1(p);
}

void E1(char* p) {
    if(*p == '+') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> +TE\'\n");
        outputCur(p);
        printf("=================================================\n");
        advance(p);
        T(p);
        E1(p);
    }
    else if(*p == '-') {
        printf("=================================================\n");
        printf("使用的产生式：E\' --> -TE\'\n");
        outputCur(p);
        printf("=================================================\n");
        advance(p);
        T(p);
        E1(p);
    }
    else{
        printf("=================================================\n");
        printf("使用的产生式：E\' --> ε\n");
        outputCur(p);
        printf("=================================================\n");
    }
}

void T(char* p) {
    printf("=================================================\n");
    printf("使用的产生式：T --> FT\'\n");
    outputCur(p);
    printf("=================================================\n");
    F(p);
    T1(p);
}

void T1(char* p) {
    if(*p == '*') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        outputCur(p);
        printf("=================================================\n");
        advance(p);
        F(p);
        T1(p);
    }
    else if(*p == '/') {
        printf("=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        outputCur(p);
        printf("=================================================\n");
        advance(p);
        F(p);
        T1(p);
    }
    else{
        printf("=================================================\n");
        printf("使用的产生式：T\' --> ε\n");
        outputCur(p);
        printf("=================================================\n");
    }
}

void F(char* p) {
    if(*p == '(') {
        printf("=================================================\n");
        printf("使用的产生式：F --> (E)\n");
        outputCur(p);
        printf("=================================================\n");
        advance(p);
        E(p);
        if(*p == ')') {
            advance(p);
        }
        else {
            //printf("\n***%c***\n", *p);
            error();
        }
    }
    else if( (*p>='a' && *p<='z') || (*p>='A' && *p<='Z') || (*p>='0' && *p<='9') ) {
        printf("=================================================\n");
        printf("使用的产生式：F --> %c\n", *p);
        outputCur(p);
        printf("=================================================\n");
        advance(p);
    }
    else{
        //printf("\n$$$%c$$$\n", *p);
        error();
    }
}

void error(){
    printf("ERROR\n");
    exit(-1);
}