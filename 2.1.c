#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void myThread1(void){
    printf("This is pthread 1.\n");
}

void myThread2(void){
    printf("This is pthread 2.\n");
}

int main(void){
    pthread_t id1,id2;
    int ret1,ret2;
    ret1=pthread_create(&id1,NULL,(void*)myThread1,NULL);
    if(0!=ret1){
        printf("Create pthread 1 error!\n");
        exit(1);
    }
    ret2=pthread_create(&id2,NULL,(void*)myThread2,NULL);
    if(0!=ret2){
        printf("Create pthread 2 error!\n");
        exit(2);
    }
    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    return(0);
}

