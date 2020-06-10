#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
main()
{
//    printf("Executing ls\n");
//    execl("/bin/ls","ls","-l",NULL);
//    perror("execl failed to run ls");
//    exit(1);


    char* av[] = {"ls","-l",NULL};
    execv("/bin/ls",av);
    perror("execv failed");
    exit(1);
}
