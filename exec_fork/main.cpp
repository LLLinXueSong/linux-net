#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
main(){
    int pid;
    pid = fork();
    switch(pid){
        case -1:
                perror("fork failed");
                exit(1);
        case 0:
            execl("/bin/ls","ls","-l",NULL);
            perror("execl failed");
            exit(1);
        default:
            wait(NULL);
            printf("ls complete\n");
            exit(0);
    }
}
