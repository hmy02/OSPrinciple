#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>

int pid1,pid2;    //存储两个子进程的PID号；
/*************************主函数**********************/
int main()
{
    int fd[2];                     //打开文件的文件描述符
      char OutPipe[100],InPipe[100]; //存储要写入管道的字符串
      pipe(fd);                      //创建管道---建立一无名管道
    while((pid1 = fork()) == -1);  //创建子进程
    if(pid1 == 0)                  //子进程
    {
        lockf(fd[1],1,0);          //lockf(fd,1,0)给文件上锁，使实现互斥访问
        sprintf(OutPipe,"Child 1 is sending message!");  //将字符串输出到OutPipe(目的字符串)中
        write(fd[1],OutPipe,50);   //从管道的写入端（句柄1）将50个字节数据写入管道
        lockf(fd[1],0,0);          //lockf(fd,0,0)是文件解锁
        sleep(1);                  //延时1秒
        exit(0);                   //子进程结束
    }
    else
    {
        while((pid2 = fork()) == -1);  //创建子进程
        if(pid2 == 0)
        {
            sleep(0);
            lockf(fd[1],1,0);          //lockf(fd,1,0)给文件上锁，使实现互斥访问
            sprintf(OutPipe,"Child 2 is sending message!");    //将字符串输出到OutPipe(目的字符串)中
            write(fd[1],OutPipe,50);  //从管道的写入端（句柄1）将50个字节数据写入管道
            lockf(fd[1],0,0);         //lockf(fd,0,0)是文件解锁
            sleep(1);
            exit(0);                  //子进程结束
        }
        else                          //父进程
        {
            wait(0);                  //等待子进程执行完毕
            wait(0);                  //等待子进程执行完毕
            read(fd[0],InPipe,50);    //从管道的读出端（句柄0）读出50个字节数据
            printf("%s\n",InPipe);    //父进程将字符串显示在屏幕上
            //wait(0);
            read(fd[0],InPipe,50);
            printf("%s\n",InPipe);
            exit(0);                  //或者 return 0;
        }
    }       
    return 0;
}
