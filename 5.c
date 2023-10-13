#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
                
int pid;
void func(int i)
{
        printf("pid= %d\n",pid);
        if(i==SIGUSR1)
                printf("received SIGUSR1\n");
        else if(i==SIGUSR2)
                printf("received SIGUSR2\n");
        else if(i==SIGINT){
                printf("received SIGINT %d\n",i);
                exit(1);}
        exit(0);
}

int main(){
        if((pid=fork())==0){
                printf("child pid= %d\n",pid);
                signal(SIGUSR1,func);
                for(;;);
                pause();
        }else{
                printf("father pid= %d\n",pid);
                signal(SIGINT,func);
                for(;;);
                pause();
        }
}       
