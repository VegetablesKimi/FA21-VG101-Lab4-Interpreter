#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Varible.h"
#include "Expr.h"
#include "Function.h"
#include <ctype.h>
#include <stdio.h>

#define CMDL (200)

char *readspace(char *str)//as it means, read space
{
    while (isspace(*str)) ++str;
    return str;
}

/*
int isres(char ch)
{
    switch (ch)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '(':
        case ')':
        case ',':
        case ';':
        case '=':
        return 1;
    }
    return 0;
}
*/

floatT getval(char *str,char **ss)
{
    floatT res_i=0,res_d=0;
    int sgn=1;
    while (!isdigit(*str) && *str != '.')
    {
        str=readspace(str);
        if (*str == '-') sgn*=-1,++str;
        else if (*str == '+') ++str;
    }
    while (isdigit(*str))
    {
        res_i=res_i*10+(*str-'0');
        ++str;
    }
    if (*str == '.')
    {
        ++str;
        double mul=0.1;
        while (isdigit(*str))
        {
            res_d=res_d+(*str-'0')*mul;
            ++str; mul*=0.1;
        }
    }
    if (ss!=NULL) *ss=str;
    return sgn*(res_i+res_d);
}

//turn infix expression into reverse Polish expression
int covexpr(char *s,floatT *arg,int *opt,floatT **adr,char **sss)
{
    char *ss;
    char v[VARL];
    int st1[EXPL];
    int tp1=0,tp2=0;
    int printres=1;

    int isexpbegin=1;
    st1[tp1++]=optid('(');

    int layer=1;
    while (1)
    {
        s=readspace(s);
        if (*s=='\0' || *s==',') break;
        if (*s==';')
        {
            printres=0;
            break;
        }
        if (*s=='(')
        {
            st1[tp1++]=optid(*s);
            ++s;
            isexpbegin=1;
            ++layer;
            continue;
        }
        else if (*s==')')
        {
            if (layer==1) break;
            while (st1[--tp1]!=optid('(')) opt[tp2++]=st1[tp1];
            ++s;
            --layer;
        }
        //deal with operators
        //'+' and '-' can be ambiguous,
        //so add a zero as the first operand when they're signs
        else if (optid(*s)>=0)
        {
            int id=optid(*s);
            if (isexpbegin)
            {
                switch (optpri(id))
                {
                    case 2: arg[tp2]=0; break;
                    case 3: arg[tp2]=1; break;
                    case 4: arg[tp2]=M_E; break;
                    default: arg[tp2]=0;
                }
                opt[tp2]=-1;
                ++tp2;
            }
            if (id!=optid('^'))
                while (optpri(st1[tp1-1])>=optpri(id))
                {
                    opt[tp2++]=st1[--tp1];
                }
            else
                while (optpri(st1[tp1-1])>optpri(id))
                {
                    opt[tp2++]=st1[--tp1];
                }
            st1[tp1++]=id;
            if (id==optid('='))
            {
                isexpbegin=1;
                ++s;
                continue;
            }
            ++s;
        }
        //deal with varibles
        else if (isvarbeginchar(*s))
        {
            int i=0;
            while (isvarchar(*s)) v[i++]=*(s++);
            v[i]='\0';
            adr[tp2]=varval+varkey(v);
            arg[tp2]=*adr[tp2];
            opt[tp2]=-2;
            ++tp2;
        }
        //deal with values
        else
        {
            arg[tp2]=getval(s,&ss);
            s=ss;
            opt[tp2]=-1;
            ++tp2;
        }
        isexpbegin=0;
    }

    while (tp1>0 && st1[--tp1]!=optid('(')) opt[tp2++]=st1[tp1];
    opt[tp2]=-17;
    if (sss!=NULL) *sss=s;
    return printres;
}

int runcmd(char *s)
{
    s=readspace(s);
    if (!isvarbeginchar(*s) || funcid(s)<0)
    {
        int printres=covexpr(s,arg,opt,adr,NULL);
        floatT res=calexpr(arg,opt,adr);
        if (printres)
            printf("%lf\n",res);
    }
    else
    {
        int key=funcid(s);
        while (isvarchar(*s)) ++s;
        switch (key)
        {
            //fprintf
            case 16327553:
            {
                s=readspace(s);
                ++s;    //read '('
                s=readspace(s);

                ++s;    //read left "
                int i;
                for (i=0;*s!='"' && *s!='\0';++i,++s) fmt[i]=*s;
                fmt[i]='\0';
                ++s;    //read right "

                i=0;
                while (1)
                {
                    s=readspace(s);
                    if (*s==')' || *s=='\0') break;
                    ++s;    //read ,
                    char *nxt;
                    covexpr(s,arg,opt,adr,&nxt);
                    s=nxt;
                    dat[i++]=calexpr(arg,opt,adr);
                }
                return fp(fmt,dat);
            }
            break;
            //labNB
            case 10415495:
                return confirmnb();
            break;
        }
    }
    return 0;
}

#endif