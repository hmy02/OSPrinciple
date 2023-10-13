#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<errno.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>

#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>

struct msgbuf
{
        long mtype;
        char mtext[64];
};

int main(int argc, char *argv[])
{
        key_t key;
        key = ftok(".",10);
        
        int msgid;
        msgid = msgget(key,IPC_CREAT|0666);
        
        struct msgbuf buf;
        
        pid_t pid = fork();
        
        if(pid>0){
                while(1){
                        memset(buf.mtext,0,sizeof(buf.mtext));
                        
                        buf.mtype = 1;
                        fgets(buf.mtext,64,stdin);
                        
                        msgsnd(msgid,&buf,strlen(buf.mtext),0);
                        
                        if(strncmp(buf.mtext,"exit",4)==0){
                                printf("bye~\n");
                                kill(pid,SIGKILL);
                                break;
                        }
                }
                wait(NULL);
        }
        
        if(pid==0){
                while(1){
                        memset(buf.mtext,0,sizeof(buf.mtext));
                        msgrcv(msgid,&buf,64,1,0);
                        printf("receive from sender:%s",buf.mtext);
                }
                wait(NULL);
        }
        
        msgctl(msgid,IPC_RMID,NULL);
        return 0;
}
