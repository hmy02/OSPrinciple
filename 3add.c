#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h> //增加的头文件，否则sleep(1)会报错
#define MAXSTACK 100 //定义最大容量为100
int stack[MAXSTACK][2]; //定义一个公共缓冲区
int size=0; //初始化大小
sem_t sem1,sem2,sem3,sem4; /*定义信号量*/

/*从文件file1.dat 读取数据，每读一次，信号量加一*/
void RData1(void)
{
    FILE *fp=fopen("1.dat","r"); //打开文件file1.dat
    while(!feof(fp)) //未到文件尾时继续执行，否则退出循环
    {
        sem_wait(&sem1); //阻塞当前线程直到信号量sem1的值大于0
        fscanf(fp,"%d %d",&stack[size][0],&stack[size][1]); //将文件中的数据读入二维数组中
        size++; //长度加一
        sem_post(&sem2); //增加信号量sem2的值
    }
    fclose(fp); //关闭文件
}

/*从文件file2.dat 读取数据，每读一次，信号量加一*/
void RData2(void)
{
    FILE *fp=fopen("2.dat","r"); //打开文件file2.dat
    while(!feof(fp)) //未到文件尾时继续执行，否则退出循环
    {
        sem_wait(&sem2); //阻塞当前线程直到信号量sem2的值大于0
        fscanf(fp,"%d %d",&stack[size][0],&stack[size][1]); //将文件中的数据读入二维数组中
        size++; //长度加一
        sem_post(&sem3); //增加信号量sem3的值
    }
    fclose(fp); //关闭文件
}

/*阻塞等待缓冲区有数据，读取数据后，释放空间，继续等待*/
void CData1(void)
{
    while(1)
    {
        sem_wait(&sem3); //阻塞当前线程直到信号量 sem 的值大于 0
        size--; //长度减一
        /*将二维数组中的数据相加*/
        printf("Plus:%d+%d=%d\n",stack[0][0],stack[1][0],stack[0][0]+stack[1][0]);
        sem_post(&sem4);
        //sleep(1); //暂停执行
    }   
}

void CData2(void)
{
    while(1)
    {
        sem_wait(&sem4); //阻塞当前线程直到信号量 sem 的值大于 0
        size--; //长度减一
        /*将二维数组中的数据相乘*/
        printf("Multiply:%d*%d=%d\n",stack[0][1],stack[1][1],stack[0][1]*stack[1][1]);
        sem_post(&sem1);
        //sleep(1); //暂停执行
    }
}

int main(void)
{
    pthread_t tid1,tid2,tid3,tid4;
    sem_init(&sem1,0,1); /*信号量初始化*/
    sem_init(&sem2,0,0);
    sem_init(&sem3,0,0);
    sem_init(&sem4,0,0);
    /*创建4个线程*/
    pthread_create(&tid1,NULL,(void *)RData1,NULL);
    pthread_create(&tid2,NULL,(void *)RData2,NULL);
    pthread_create(&tid3,NULL,(void *)CData1,NULL);
    pthread_create(&tid4,NULL,(void *)CData2,NULL);
    
    /*等待线程结束*/
    pthread_join(tid1,NULL);
    //pthread_join(tid2,NULL);
    //pthread_join(tid3,NULL);
    //pthread_join(tid4,NULL);
    
    return 0;
}
