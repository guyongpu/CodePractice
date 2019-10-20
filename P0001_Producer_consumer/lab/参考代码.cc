//URL： https://blog.csdn.net/ywcpig/article/details/52598880

//信号量---线程间通信
//“生产者消费者” 问题
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define msleep(x) usleep(x*1000)
#define PRODUCT_SPEED 1     //生产速度
#define CONSUM_SPEED  3     //消费速度
#define INIT_NUM      3     //仓库原有产品数
#define TOTAL_NUM     10    //仓库容量
 
sem_t p_sem, c_sem, sh_sem;
int num=INIT_NUM;
 
//生产产品
void product(void)
{
    sleep(PRODUCT_SPEED);
}
 
//添加产品到仓库
int add_to_lib()
{
    num++;                  //仓库中的产品增加一个
    msleep(500);
    return num;
}
 
//消费
void consum()
{
    sleep(CONSUM_SPEED);
}
 
//从仓库中取出产品
int sub_from_lib()
{
    num--;                  //仓库中的产品数量减一
    msleep(500);
    return num;
}
 
//生产者线程
void *productor(void *arg)
{
    while(1)
    {
        sem_wait(&p_sem);   //生产信号量减一
        product();          //生产延时
        sem_wait(&sh_sem);  //这个信号量是用来互斥的
        printf("push into! tatol_num=%d\n", add_to_lib());
        sem_post(&sh_sem);
        sem_post(&c_sem);   //消费信号量加一
    }
}
 
//消费者线程
void *consumer(void *arg)
{
    while(1)
    {
 
        sem_wait(&c_sem);   //消费者信号量减一
        sem_wait(&sh_sem);
        printf("pop out! tatol_num=%d\n", sub_from_lib());
        sem_post(&sh_sem);
        sem_post(&p_sem);   //生产者信号量加一
        consum();           //消费延时
    }
}
 
int main()
{
    pthread_t tid1,tid2;
 
    sem_init(&p_sem,0,TOTAL_NUM-INIT_NUM);
    sem_init(&c_sem,0,INIT_NUM);
    sem_init(&sh_sem,0,1);
 
    pthread_create(&tid1,NULL,productor,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);
 
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
 
    return 0;
}