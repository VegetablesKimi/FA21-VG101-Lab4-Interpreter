#ifndef _VARIBLE_H
#define _VARIBLE_H

#include <string.h>
#include <ctype.h>

#define MAGIC (19260817)//a prime number used to reduce the probability of Hash collision
#define VARL (16)
#define isvarchar(ch) (isalnum(ch) || (ch)=='_')//isalnum is used to judge whether it is a character or a decimal number
#define isvarbeginchar(ch) (isalpha(ch) || (ch)=='_')//isalpah is used to judge whether it is a character
//the above two is used to determine whether a variavle name is valid, and the second is used to check whether the first character of a variable is valid

typedef double floatT;//floatT -> dpuble

char varname[MAGIC][VARL];
floatT varval[MAGIC];

//varible names are assigned into a hash table
//get the hash key of a string
int strhash(char *s)
{
	int res=0;
	for (;isvarchar(*s);++s) res=(res*47+(*s))%MAGIC;
	return res;
}

//get the value of a varible, unused
floatT queryvar(char *s,int *k)
{
	int key=strhash(s);
	while (varname[key][0] != '\0' && strcmp(s,varname[key])) key=(key+1)%MAGIC;
    if (k!=NULL) *k=varname[key][0];
	return varval[key];
}

//change the value of a varible, unused
void mdfvar(char *s,floatT x)
{
    int key=strhash(s);
	while (varname[key][0] != '\0' && strcmp(s,varname[key])) key=(key+1)%MAGIC;
    if (!varname[key][0])
        strcpy(varname[key],s);
    varval[key]=x;
}

//get the key of a varible
int varkey(char *s)
{
	int key=strhash(s);
	while (varname[key][0] != '\0' && strcmp(s,varname[key])) key=(key+1)%MAGIC;
    if (!varname[key][0])
        strcpy(varname[key],s);
	return key;
}

#endif