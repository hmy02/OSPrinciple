#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h> //增加的头文件，否则sleep(1)会报错
#include<wait.h>
#define MAXSTACK 100 //定义最大容量为100
int stack[MAXSTACK]; //定义一个数组
int size=0; //初始化大小
sem_t s1,s2,s3; /*定义信号量*/
int i=0;
int j=0;
int a,b;
/*从文件file1.dat 读取数据，每读一次，信号量加一*/
void RData1(void)
{
    FILE *fp=fopen("1.dat","r"); //打开文件file1.dat
    while(!feof(fp)) //未到文件尾时继续执行，否则退出循环
    {
        sem_wait(&s3);
        fscanf(fp,"%d",&stack[size]); //将文件中的数据读入数组中
        ++size; //长度加一
        sem_post(&s1); //增加信号量的值}
    }
    fclose(fp); //关闭文件
}

/*从文件file2.dat 读取数据，每读一次，信号量加一*/
void RData2(void)
{
    FILE *fp=fopen("2.dat","r"); //打开文件file2.dat
    while(!feof(fp)) //未到文件尾时继续执行，否则退出循环
    {
        sem_wait(&s1);
        fscanf(fp,"%d",&stack[size]); //将文件中的数据读入数组中
        ++size; //长度加一
        sem_post(&s2);//增加信号量的值
    }
    fclose(fp); //关闭文件
}

/*阻塞等待缓冲区有数据，读取数据后，释放空间，继续等待*/
void CData1(void)
{
    while(1)
    {   
        sleep(1); //暂停执行
        if(i>=10) break;
        sem_wait(&s2);//阻塞当前线程直到信号量 sem 的值大于 0
        /*将数据相加*/
        --size; //长度减一
        a=stack[size];
        --size;
        b=stack[size];
        printf("Plus:%d+%d=%d\n",a,b,a+b);
        i++;
        sem_post(&s3);
    }   
}

void CData2(void)
{
    while(1)
    {
        sleep(1); //暂停执行
        if(i>=10) break; 
        sem_wait(&s2);//阻塞当前线程直到信号量 sem 的值大于 0
        /*将数据相乘*/
        --size; //长度减一
        a=stack[size];
        --size;
        b=stack[size];
        printf("Multiply:%d*%d=%d\n",a,b,a*b); 
        i++;
        sem_post(&s3);
    }
}

int main(void)
{
    pthread_t tid1,tid2,tid3,tid4;
    sem_init(&s1,0,0);
    sem_init(&s2,0,0); 
    sem_init(&s3,0,1);/*信号量初始化*/
    /*创建4个线程*/
    pthread_create(&tid1,NULL,(void *)RData1,NULL);
    pthread_create(&tid2,NULL,(void *)RData2,NULL);
    pthread_create(&tid3,NULL,(void *)CData1,NULL);
    pthread_create(&tid4,NULL,(void *)CData2,NULL);
    
    /*等待线程结束*/
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    //pthread_join(tid3,NULL);
    //pthread_join(tid4,NULL);
    
    return 0;
}
