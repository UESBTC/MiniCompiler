//
//  main.c
//  PredictSet
//
//  Created by 陈泽宁 on 2018/12/9.
//  Copyright © 2018 陈泽宁. All rights reserved.
//

#include <stdio.h>
#define MAX 1000
char Queue[MAX];
char Stack[MAX];
int head=0;
int tail=0;
int top=0;
struct PredicSet    {
    char left;
    char right[10];
    int rlength;
} predicSet[5][8];
void addQueue(char c) {
    Queue[head++]=c;
}
void QueueInitial() {
    char c;
    while ((c=getchar())!='\n') {
        addQueue(c);
    }
}
char outQueue() {
    return Queue[tail++];
}
void pushStack(char c)    {
    Stack[top++]=c;
}
char outStack() {
    return Stack[--top];
}
void StackInitial() {
    pushStack('#');
    pushStack('S');
}
void PredicSetInitial() {
    predicSet[0][2].left='S';
    predicSet[0][2].right[0]='T';
    predicSet[0][2].right[1]='B';
    predicSet[0][2].rlength=2;
    
    predicSet[0][3].left='S';
    predicSet[0][3].right[0]='T';
    predicSet[0][3].right[1]='B';
    predicSet[0][3].rlength=2;
    
    predicSet[0][5].left='S';
    predicSet[0][5].right[0]='T';
    predicSet[0][5].right[1]='B';
    predicSet[0][5].rlength=2;
    
    predicSet[0][6].left='S';
    predicSet[0][6].right[0]='T';
    predicSet[0][6].right[1]='B';
    predicSet[0][6].rlength=2;
    
    predicSet[1][1].left='B';
    predicSet[1][1].right[0]='b';
    predicSet[1][1].right[1]='T';
    predicSet[1][1].right[2]='B';
    predicSet[1][1].rlength=3;
    
    predicSet[1][4].left='B';
    predicSet[1][4].right[0]='~';
    predicSet[1][4].rlength=1;
    
    predicSet[1][7].left='B';
    predicSet[1][7].right[0]='~';
    predicSet[1][7].rlength=1;
    
    predicSet[2][2].left='T';
    predicSet[2][2].right[0]='F';
    predicSet[2][2].right[1]='A';
    predicSet[2][2].rlength=2;
    
    predicSet[2][3].left='T';
    predicSet[2][3].right[0]='F';
    predicSet[2][3].right[1]='A';
    predicSet[2][3].rlength=2;
    
    predicSet[2][5].left='T';
    predicSet[2][5].right[0]='F';
    predicSet[2][5].right[1]='A';
    predicSet[2][5].rlength=2;
    
    predicSet[2][6].left='T';
    predicSet[2][6].right[0]='F';
    predicSet[2][6].right[1]='A';
    predicSet[2][6].rlength=2;
    
    predicSet[3][0].left='A';
    predicSet[3][0].right[0]='a';
    predicSet[3][0].right[1]='F';
    predicSet[3][0].right[2]='A';
    predicSet[3][0].rlength=3;
    
    predicSet[3][1].left='A';
    predicSet[3][1].right[0]='~';
    predicSet[3][1].rlength=1;
    
    predicSet[3][4].left='A';
    predicSet[3][4].right[0]='~';
    predicSet[3][4].rlength=1;
    
    predicSet[3][7].left='A';
    predicSet[3][7].right[0]='~';
    predicSet[3][7].rlength=1;
    
    predicSet[4][2].left='F';
    predicSet[4][2].right[0]='n';
    predicSet[4][2].right[1]='F';
    predicSet[4][2].rlength=2;
    
    predicSet[4][3].left='F';
    predicSet[4][3].right[0]='(';
    predicSet[4][3].right[1]='S';
    predicSet[4][3].right[2]=')';
    predicSet[4][3].rlength=3;
    
    predicSet[4][5].left='F';
    predicSet[4][5].right[0]='t';
    predicSet[4][5].rlength=1;
    
    
    predicSet[4][6].left='F';
    predicSet[4][6].right[0]='f';
    predicSet[4][6].rlength=1;
}
int encoder(char c)    {
    switch (c) {
        case 'S':
        case 'a':
            return 0;
            break;
        case 'B':
        case 'b':
            return 1;
            break;
        case 'T':
        case 'n':
            return 2;
            break;
        case 'A':
        case '(':
            return 3;
            break;
        case 'F':
        case ')':
            return 4;
            break;
        case 't':
            return 5;
            break;
        case 'f':
            return 6;
            break;
        case '#':
            return 7;
            break;
        default:
            return -1;
    }
}
int isNonTermin(char c)    {
    if (c>='A'&&c<='Z') {
        return 1;
    }
    return 0;
}
void Solver()   {
    char oriR,oriL;
    while(1) {
        oriL=outStack();
        oriR=Queue[tail];
        if(isNonTermin(oriL)) {
            if (predicSet[encoder(oriL)][encoder(oriR)].right[0]!='~') {
                int i=predicSet[encoder(oriL)][encoder(oriR)].rlength-1;
                for (; i>=0; i--) {
                    pushStack(predicSet[encoder(oriL)][encoder(oriR)].right[i]);
                }
            }
        }
        else    {
            if(oriL!=oriR)  {
                printf("错误\n");
                break;
            }
            else if(oriL=='#'&&oriR=='#')   {
                printf("正确\n");
                break;
            }
            else    {
                outQueue();
            }
        }
    }
}
int main(int argc, const char * argv[]) {
    printf("Please Input:");
    QueueInitial();
    StackInitial();
    PredicSetInitial();
    Solver();
    return 0;
}
