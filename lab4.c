#include "Interpreter.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG (0)
#define FINPUT (1)
//1 for file input, 0 for keyboard input

int main()
{
    FILE *fin=FINPUT?fopen("command.txt","r"):stdin;
    funcinit(DEBUG);
    char cmd[CMDL+1];
    while (fgets(cmd,CMDL,fin))
    {
        int stat=runcmd(cmd);
        if (DEBUG && stat) printf("This command exited with error code %d\n",stat);
    }
    fclose(fin);
    return 0;
}