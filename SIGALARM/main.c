#include <stdio.h>
#include <stdlib.h>
#include<signal.h>

#define TIMEOUT 5
#define MAXTRIES 3
#define LINESIZE 100
#define BELL '\007'
#define TRUE 1
#define FALSE 0
static int time_out;
static char inputline[LINESIZE];
char* quickreply(char* prompt);
int main()
{
    printf("%s\n",quickreply("Input"));
    return 0;
}

char* quickreply(char* prompt)
{
    int (*was)(),catch(),ntries;
    char* answer;
    was = signal(SIGALRM,catch);
    for(ntries = 0;ntries<MAXTRIES;ntries++)
    {
        time_out = FALSE;
        printf("\n%s>",prompt);
        alarm(TIMEOUT);
        answer = gets(inputline);
        alarm(0);
        if(!time_out)
            break;
    }
    signal(SIGALRM,was);
    return (time_out?((char*)0):answer);
}
catch()
{
    time_out=TRUE;
    putchar(BELL);
}
