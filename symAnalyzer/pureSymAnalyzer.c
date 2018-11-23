#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXQUEUE 200
void E();
void E1();
void T();
void T1();
void F();
void error();
void advance();
char Queue[MAXQUEUE];
char sym;
FILE* inFile;
FILE* outFile;
FILE* errorout;
int head = 0, end = 0;
void InitQueue();
void AddQueue();
void OutQueue();
void OutPutCur();
int main(int argc,const char * argv[])  {
    inFile = fopen(argv[1],"r");
    outFile = fopen("detail.txt", "w");
    errorout = fopen("error.txt", "w");

    if (inFile == NULL){
        printf("ERROR:can not open file.\n");
        fprintf(errorout, "ERROR:can not open file.\n");
        exit(-1);
    }
    InitQueue();
    OutQueue();
    E();
    
}
void OutPutCur()    {
    printf("已分析过的串：");
    fprintf(outFile, "已分析过的串：");
    
    for(int i=0;i<head-1;i++) {
        printf("%c ", Queue[i]);
        fprintf(outFile,"%c ", Queue[i]);
    }
    
    printf("\n当前字符：%c\n", sym);
    fprintf(outFile,"\n当前字符：%c\n", sym);
    
    printf("剩余串：");
    fprintf(outFile, "剩余串：");
    for(int i=head;i<end;i++) {
        printf("%c ", Queue[i]);
        fprintf(outFile, "%c ", Queue[i]);
    }
    printf("\n");
    fprintf(outFile,"\n");
}
void advance() {
    sym = getc(inFile);
    if (sym == ' ' || sym == '\t'){
        while(sym == ' ' || sym == '\t')
            sym = getc(inFile);
    }
}
void InitQueue()    {
    advance();
    while (sym!=EOF) {
        AddQueue();
        advance();
    }
}
void AddQueue() {
    Queue[end++]=sym;
}
void OutQueue()  {
    sym=Queue[head++];
}
void E() {
    printf("=================================================\n");
    fprintf(outFile, "=================================================\n");
    printf("使用的产生式：E --> TE\'\n");
    fprintf(outFile, "使用的产生式：E --> TE\'\n");
    OutPutCur();
    printf("=================================================\n");
    fprintf(outFile, "=================================================\n");
    T();
    E1();
}

void E1() {
    if(sym == '+') {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：E\' --> +TE\'\n");
        fprintf(outFile, "使用的产生式：E\' --> +TE\'\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        OutQueue();
        T();
        E1();
    }
    else if(sym == '-') {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：E\' --> -TE\'\n");
        fprintf(outFile, "使用的产生式：E\' --> -TE\'\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        OutQueue();
        T();
        E1();
    }
    else{
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：E\' --> ε\n");
        fprintf(outFile, "使用的产生式：E\' --> ε\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
    }
}

void T() {
    printf("=================================================\n");
    fprintf(outFile, "=================================================\n");
    printf("使用的产生式：T --> FT\'\n");
    fprintf(outFile, "使用的产生式：T --> FT\'\n");
    OutPutCur();
    printf("=================================================\n");
    fprintf(outFile, "=================================================\n");
    F();
    T1();
}

void T1() {
    if(sym == '*') {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        fprintf(outFile, "使用的产生式：T\' --> *FT\'\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        OutQueue();
        F();
        T1();
    }
    else if(sym == '/') {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：T\' --> *FT\'\n");
        fprintf(outFile, "使用的产生式：T\' --> *FT\'\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        OutQueue();
        F();
        T1();
    }
    else{
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：T\' --> ε\n");
        fprintf(outFile, "使用的产生式：T\' --> ε\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
    }
}

void F() {
    if(sym == '(') {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：F --> (E)\n");
        fprintf(outFile, "使用的产生式：F --> (E)\n");
        OutPutCur();
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        OutQueue();
        E();
        if(sym == ')') {
            OutQueue();
        }
        else {
            error();
        }
    }
    else if(isdigit(sym)) {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：F --> i\n");
        fprintf(outFile, "使用的产生式：F --> i\n");
        printf("已分析过的串：");
        fprintf(outFile, "已分析过的串：");
        for(int i=0;i<head-1;i++) {
            printf("%c ", Queue[i]);
            fprintf(outFile, "%c ", Queue[i]);
        }
            
        
        printf("\n当前字符：");
        fprintf(outFile, "\n当前字符：");
        while (isdigit(sym)) {
            printf("%c ", sym);
            fprintf(outFile, "%c ", sym);
            OutQueue();
        }
        printf("\n剩余串：");
        fprintf(outFile, "\n剩余串：");
        for(int i=head; i<end; i++) {
            printf("%c ", Queue[i]);
            fprintf(outFile, "%c ", Queue[i]);
        }
            
        printf("\n=================================================\n");
        fprintf(outFile, "\n=================================================\n");
    }
    else if(isalpha(sym))   {
        printf("=================================================\n");
        fprintf(outFile, "=================================================\n");
        printf("使用的产生式：F --> i\n");
        fprintf(outFile, "使用的产生式：F --> i\n");
        printf("已分析过的串：");
        fprintf(outFile, "已分析过的串：");
        for(int i=0;i<head-1;i++) {
            printf("%c ",Queue[i]);
            fprintf(outFile, "%c ",Queue[i]);
        }
        printf("\n当前字符：");
        fprintf(outFile, "\n当前字符：");
        while (isalpha(sym)) {
            printf("%c ",sym);
            fprintf(outFile, "%c ",sym);
            OutQueue();
        }
        printf("\n剩余串：");
        fprintf(outFile, "\n剩余串：");
        for(int i=head;i<end;i++) {
            printf("%c ",Queue[i]);
            fprintf(outFile, "%c ",Queue[i]);
        }
        printf("\n=================================================\n");
        fprintf(outFile, "\n=================================================\n");
    }
    else{
        error();
    }
}
void error(){
    printf("ERROR\n");
    fprintf(outFile, "ERROR\n");
    fprintf(errorout, "ERROR\n");
    exit(-1);
}
