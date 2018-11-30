//
//  main.cpp
//  FirstFollowCollection
//
//  Created by 陈泽宁 on 2018/11/27.
//  Copyright © 2018 陈泽宁. All rights reserved.
//

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include<string.h>

#define MAX 100
#define MaxVtNum 20    /*终结符最大的数目*/
#define MaxVnNum 20    /*非终结符最大的数目*/
#define MaxPLength 20    /*产生式右部最大长度*/


struct product            /*产生式类型定义*/
{
    int length;            /*产生式右部的文法符号串的长度*/
    char left;            /*产生式左部的非终结符*/
    char right[MaxPLength];                /*产生式右部的文法符号串*/
}p[100];                /*产生式结构体*/

struct set
{
    int n=0;                /*FIRST集和FOLLOW集中元素的个数*/
    char elm[100];
}first[MAX], follow[MAX];    /*FIRST集和FOLLOW集结构体*/

char termin[MaxVtNum];        /*终结符*/
char non_termin[MaxVnNum];    /*非终结符*/

int n;                        /*产生式数量*/
int VtNum;                    /*终结符数量*/
int VnNum;                    /*非终结符数量*/

/*判断是否为终结符*/
int isterminal(char x)
{
    if (x >= 'A'&& x <= 'Z')    return 0;
    return 1;
}

/*判断符号x是否出现过*/
int exist(char x)
{
    int i;
    if (isterminal(x))
    {
        for (i = 1; i <= VtNum; i++)
            if (termin[i] == x) return 1;
        return 0;
    }
    
    for (i = 1; i <= VnNum; i++)
        if (non_termin[i] == x) return 1;
    return 0;
}

/*读入文法*/
void read()                /*统计文法中的终结符以及非终结符，按照出现顺序添加到对应的集合中*/
{
    int i, j, k;
    char tmp[25];
    printf("输入产生式的数量：");
    scanf("%d", &n);
    printf("输入形如A->abc的产生式，其中~表示空：\n");
    for (VnNum = VtNum = 0, i = 1; i <= n; i++)
    {
        scanf(" %s", tmp);
        p[i].left = tmp[0];
        
        if (!exist(tmp[0])) non_termin[++VnNum] = tmp[0];
        
        for (k=0,j=3;tmp[j];j++)
        {
            p[i].right[k++] = tmp[j];
            if (isterminal(tmp[j]))
                if (!exist(tmp[j])) termin[++VtNum] = tmp[j];
                else
                    if (!exist(tmp[j])) non_termin[++VnNum] = tmp[j];
        }
        p[i].right[k] = 0;
        p[i].length = k - 1;
    }
    termin[++VtNum] = non_termin[++VnNum] = '#';
}

/*输出用户所输入的文法*/
void show()
{
    int i;
    printf("用户所输入的产生式为：\n");
    for (i = 1; i <= n; i++)
        printf("(%d)    %c->%s\n", i, p[i].left, p[i].right);
}

/*把符号x变为对应的编号*/
int char_id(char x)
{
    int i;
    if (!isterminal(x))                /*当前符号为非终结符*/
    {
        for (i = 1; i <= VnNum; i++)
            if (non_termin[i] == x) return i;
    }
    for (i = 1; i <= VtNum; i++)
        if (termin[i] == x) return i + 1000;
    return -1;
}
/*判断符号idt是否在集合st里面*/
int in(struct set &st, char id)
{
    int i;
    for (i = 1; i <= st.n; i++)
    {
        if (st.elm[i] == id)
        {
            return 1;
        }
    }
    return 0;
}

/*把符号e添加到集合st里面*/
void add(struct set &st, char e)
{
    st.n++;
    st.elm[st.n] = e;
}

/*求FIRST集*/
void compute_first()
{
    int i, j, k=1, idl, idr;
    bool inc;
    inc = true;
    while (inc)
    {
        inc = false;
        for (i = 1; i <= n; i++)        /*遍历所有产生式*/
        {
            printf("This is the %d product\n", i);
            /*
             idl为每个产生式左部的非终结符
             */
            idl = char_id(p[i].left);
            for (j = 0; p[i].right[j]; j++)            /*判断产生式右部为终结符还是非终结符*/
            {
                /*
                 idr为产生式右边的第j个字符
                 */
                idr = char_id(p[i].right[j]);
                printf("Current charecter is %c\n", p[i].right[j]);
                if (idr > 1000)            /*如果当前为终结符，并且first[idl]中不包含该终结符，则把此终结符加入first[idl]中*/
                {
                    if (!in(first[idl], p[i].right[j]))
                    {
                        //                        printf("Current is the terninal\n");
                        printf("Add %c into FIRST(%c)\n", p[i].right[j], non_termin[idl]);
                        add(first[idl], p[i].right[j]);
                        inc = true;
                    }
                    break;
                }
                else                   /*否则把该非终结符的first集里面的非空元素加入first[idl]中*/
                {
                    for (k = 1; k <= first[idr].n; k++)
                    {
                        if (!in(first[idl], first[idr].elm[k]) && first[idr].elm[k] != '~')
                        {
                            //                            printf("Current is the non-terninal\n");
                            printf("Add %c into FIRST(%c)\n", first[idr].elm[k], non_termin[idl]);
                            add(first[idl], first[idr].elm[k]);
                            inc = true;
                        }
                    }
                    if (!in(first[idr], '~'))//有空还能往下走。没空就8行了
                        break;
                }
            }
        }//对每个产生式循环结束
    }
    for (i = 1; i <= n; i++)        /*遍历所有产生式*/
    {
        /*
         idl为每个产生式左部的非终结符
         */
        idl = char_id(p[i].left);
        for (j = 0; p[i].right[j]; j++)            /*判断产生式右部为终结符还是非终结符*/
        {
            /*
             idr为产生式右边的第j个字符
             */
            idr = char_id(p[i].right[j]);
            if (idr > 1000)            /*如果当前为终结符，并且first[idl]中不包含该终结符，则把此终结符加入first[idl]中*/
            {
                break;
            }
            else                   /*否则把该非终结符的first集里面的非空元素加入first[idl]中*/
            {
                if (!in(first[idr], '~'))//有空还能往下走。没空就8行了
                    break;
            }
        }
        /*
         第一个条件不是很懂，我寻思着这里的“&&“是不是应该改成“||”b，即若没有右边的产生式，空就在first集里面；或者balabala
         (改了之后果然可以了
         若产生式右部的每一个文法符号都可以推导出空，则‘~’应属于first[idl]
         前面已经循环过一遍了，所以如果first集里面还是没有’空‘的话，那就说明前面的每一个文法符号里边都可以推导出空，所以p[i]的first集里面也要加‘空’
         */
        if (p[i].right[0] == 0 || (in(first[idr], '~')&&!in(first[idl], '~')))
        {
            add(first[idl], '~');
            inc = true;
        }
    }
}

/*输出每个非终结符的FIRST集*/
void print_first(struct set *st)
{
    int i, j;
    puts("\n");
    for (i = 1; i < VnNum; i++)
    {
        printf("%s(%c):    ", "FIRST", non_termin[i]);
        for (j = 1; j <= st[i].n; j++)
            printf("%c    ", st[i].elm[j]);
        printf("\n");
    }
}

void compute_fellow()   {
    int idl,idr1,idr2;
    bool inc=true;
    add(follow[char_id(p[1].left)], '#');
    while (inc) {
        inc=false;
        for (int i=1; i<=n; i++) {
            idl=char_id(p[i].left);
            for (int j=1; p[i].right[j]; j++) {
                idr1=char_id(p[i].right[j-1]);
                idr2=char_id(p[i].right[j]);
                if (idr1 < 1000) {
                    for (int k=1; k<=first[idr2].n; k++) {
                        if(first[idr2].elm[k]!='~'&&!in(follow[idr1], first[idr2].elm[k]))    {
                            add(follow[idr1], first[idr2].elm[k]);
                            inc=true;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    read();
    show();
    compute_first();
    print_first(first);
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单



