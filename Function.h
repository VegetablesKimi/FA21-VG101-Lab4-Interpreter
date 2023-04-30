#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "Varible.h"
#include <stdio.h>

#define FUNCN (2)
char funcname[FUNCN][VARL]={
    "fprintf",
    "labNB"
};
int isfunc[MAGIC];

char fmt[EXPL];
floatT dat[EXPL];

//initialize the functions
//i.e. get function id
void funcinit(int dbg)
{
    if (dbg)
        printf("Function id:\n");
    for (int i=0;i<FUNCN;++i)
    {
        int key=strhash(funcname[i]);
        while (varname[key][0]) key=(key+1)%MAGIC;
        strcpy(varname[key],funcname[i]);
        isfunc[key]=1;
        if (dbg) printf("%s : %d\n",funcname[i],key);
    }
}

int funcid(char *s)
{
    int key=strhash(s);
    if (!isfunc[key]) return -1;
    while (!strcmp(varname[key],s)) key=(key+1)%MAGIC;
    return key;
}

int confirmnb()
{
    printf("That's true. I can\'t agree more.\n");
    printf("Lab is undeinably very NB.\n");
    return 'l'+'a'+'b'+'N'+'B';
}

//formatted output
//fprintf
int fp(char *fmt,floatT *dat)//fmt: control the output format; dat: output data
{
    while (*fmt!='\0')
    {
        if (*fmt=='\\')
        {
            ++fmt;
            if (*fmt=='n') putchar('\n');
            else putchar(*fmt);
            ++fmt;
            //about the function of '\n'
        }
        else if (*fmt == '%')
        {
            ++fmt;
            if (*fmt=='f')
                printf("%lf",*dat);
            ++dat;
            ++fmt;
        }//about the function of '%f'
        else
        {
            putchar(*fmt);
            ++fmt;
        }
    }
    //putchar('\n');
    return 0;
}

#endif