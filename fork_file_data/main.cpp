#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/wait.h>
void failure(char* s)
{

    perror(s);
    exit(1);
}

void printpos(char* string,int fildes){
    long pos;
    if((pos=lseek(fildes,0L,1))<0L)
        failure("lseek failed");
    printf("%s:%ld\n",string,pos);
}

main()
{

    int fd;
    int pid;
    char buf[10];
    if((fd=open("./main.cpp",O_RDONLY))<0)
        failure("open failed");
    read(fd,buf,10);
    printpos("Before fork",fd);
    if((pid=fork())<0)
        failure("fork failed");
    else if(!pid){
        printpos("Chlid before read",fd);
        read(fd,buf,10);
        printpos("child after read",fd);
    }else{
        wait(NULL);
        printpos("parent after wait",fd);
    }

}
