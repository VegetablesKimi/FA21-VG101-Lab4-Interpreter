#ifndef _EXPR_H
#define _EXPR_H

#include <math.h>
#include "Varible.h"
#define EXPL (100)

int opt[EXPL];
floatT arg[EXPL];//store the value of parameter
floatT *adr[EXPL];//store the address of the variable

//get the id of an operator
int optid(char opt)
{
    int id=-1;
    switch (opt)//turn the operators into program internal coding so as to determine the priority
    {
        case '=': id=0x11; break;
        case '+': id=0x21; break;
        case '-': id=0x22; break;
        case '*': id=0x31; break;
        case '/': id=0x32; break;
        case '^': id=0x41; break;
        case '(': id=0x01; break;
        case ')': id=0x02; break;
    }
    return id;
}

//get the priority of an operator
int optpri(int opt)
{
    return opt>=0?(opt>>4):opt;
}

//calculation
//'=' is considered as a special operator
floatT cal(floatT *x1,floatT x2,int opt)
{
    switch (opt)
    {
        case 0x21: return *x1+x2;
        case 0x22: return *x1-x2;
        case 0x31: return *x1*x2;
        case 0x32: return *x1/x2;
        case 0x41: return pow(*x1,x2);
        case 0x11: return (*x1)=x2;
    }
    return 0;
}

//calculate reverse Polish expression
floatT calexpr(floatT *arg,int *opt,floatT **adr)
{
    int t=0;
    for (int i=0;opt[i]>=-2;++i)
        if (opt[i]<0)
        {
            arg[t]=arg[i],opt[t]=opt[i];
            if (opt[i]==-2) adr[t]=adr[i];
            ++t;
        }
        else
        {
            --t;
            if (opt[t-1]>=-1)
                arg[t-1]=cal(&arg[t-1],arg[t],opt[i]);
            else
                arg[t-1]=cal(adr[t-1],arg[t],opt[i]);
            opt[t-1]=-1;
        }
    return arg[0];
}

#endif