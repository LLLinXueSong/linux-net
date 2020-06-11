#include <stdio.h>
#include <stdlib.h>
#include<signal.h>
int ntimes = 0;
main()
{
    int pid,ppid;
    int p_action(),c_action();
    signal(SIGUSR1,p_action);
    switch(pid = fork()){
    case -1:
        perror("synchro");
        exit(1);
    case 0:
        signal(SIGUSR1,c_action);
        ppid = getppid();
        for(;;){
            sleep(1);
            kill(ppid,SIGUSR1);
            pause();
        }
        break;
    default:
        for(;;){
            pause();
            sleep(1);
            kill(pid,SIGUSR1);
        }
    }
}
p_action()
{

    printf("Patent caught signal #%d\n",++ntimes);
}
c_action()
{
    printf("Child caught signal #%d\n",++ntimes);
}
