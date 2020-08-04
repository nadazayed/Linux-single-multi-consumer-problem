//  multiple-consumer.c
//  Created by Nada AZ on 4/8/19.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int size=10;
int buffer[10]; //shared resource

sem_t s; //buffer access
sem_t n; //buffer elements
sem_t e; //buffer empty

FILE *fp, *fp2;
int producer_num=0, pitem_num=0;
int consumer_num=0, citem_num=0;

int j=0,value;
int IN=0, OUT=0;
void *producer(void *args)
{
    while (j<10000)
    {
        
        sem_wait(&e); //check empty slots
        sem_wait(&s); //check buffer buffer "mutex"
        
        /* critical section*/
        fp = fopen ("producers activity.txt","a+");
        producer_num++;
        printf("producer %d entered\n",producer_num);
        
        value=rand()% 1000;
        buffer[IN]=value;
        IN=(IN+1) % size; //for re-loop
        
        pitem_num++;
        printf("producer produced %d item \n",pitem_num);
        
        printf("producer produced %d value \n",value);
        
        fprintf(fp,"%d\t%d\t%d\n",producer_num,pitem_num,value);
        fclose (fp);
        /* critical section*/
        
        
        sem_post(&s);
        sem_post(&n);
        j++;
        //sleep(1 + rand()%3);
    }
    return NULL;
}


int value2;
void *consumer(void *args)
{
    while(1)
    {
        sem_wait(&n);
        sem_wait(&s);
        
        /* critical section*/
        fp2 = fopen ("consumer activity.txt","a+");
        
        consumer_num++;
        printf("consumer %d entered\n",consumer_num);
        
        value2=buffer[OUT];
        OUT=(OUT+1) % size;
        
        citem_num++;
        printf("consumer consumed %d item \n",citem_num);
        
        printf("consumer consumed %d value \n",value2);
            
        fprintf(fp2,"%d\t%d\t%d\n",consumer_num,citem_num,value2);
        fclose (fp2);
        /* critical section*/
        
        sem_post(&s);
        sem_post(&e);
        //z++;
        //sleep(1+rand()%5);
    }
    return NULL;
}


int main()
{
    sem_init(&s,0,1); //binary sem "access to buffer"
    sem_init(&n,0,0); //count sem "# of field slots"
    sem_init(&e,0,size); //count sem "# of empty slots"
    
    pthread_t Tproducer,Tconsumer;
    pthread_create(&Tproducer, NULL, producer, NULL);
    pthread_create(&Tconsumer, NULL, consumer, NULL);
    
    
    pthread_join(Tproducer,NULL);
    pthread_join(Tconsumer,NULL);
    
    sem_destroy(&s);
    sem_destroy(&n);
    sem_destroy(&e);
    return 0;
}


