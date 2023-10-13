#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void *myThread1(void* arg){
    int* num;
    num=(int*)arg;
    printf("create parameter is %d.\n",*num);
    return (void*)0;
}

void *myThread2(void* arg){
    char* ch;
    ch=(char*)arg;
    printf("create parameter is %c.\n",*ch);
    return (void*)0;
}

int main(void){
    pthread_t id1,id2;
    int ret1,ret2;
    int num=1;
    char ch='a';
    int* p_num=&num;
    char* p_ch=&ch;
    ret1=pthread_create(&id1,NULL,myThread1,(void*)p_num);
    if(0!=ret1){
        printf("Create pthread 1 error!\n");
        exit(1);
    }
    ret2=pthread_create(&id2,NULL,myThread2,(void*)p_ch);
    if(0!=ret2){
        printf("Create pthread 2 error!\n");
        exit(2);
    }
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    return(0);
}
