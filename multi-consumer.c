//  multiple-consumer.c
//  Created by Nada AZ on 4/8/19.

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

int size=10;
int buffer[10]; //shared resource

sem_t s; //buffer access

sem_t n1; //buffer elements
sem_t n2;
sem_t n3;
sem_t n4;

sem_t e1; //buffer empty
sem_t e2;
sem_t e3;
sem_t e4;

FILE *fp1, *fp2;
int producer_num=0, pitem_num=0;

int j=0,value;
int IN=0;
void *producer(void *args)
{
    while (j<10000)
    {
        
        sem_wait(&e1); //check empty slots
        sem_wait(&e2);
        sem_wait(&e3);
        sem_wait(&e4);
        
        sem_wait(&s); //check buffer buffer "mutex"
        
        /* critical section*/
        fp1 = fopen ("producers activity.txt","a+");
        producer_num++;
        printf("producer %d entered\n",producer_num);
        
        value=rand()% 1000;
        buffer[IN]=value;
        
        pitem_num++;
        printf("producer produced %d item \n",pitem_num);
        
        printf("producer produced %d value \n",value);
        
        fprintf(fp1,"%d\t%d\t%d\t%d\n",producer_num,IN,pitem_num,value);
        IN=(IN+1) % size; //for re-loop
        fclose (fp1);
        /* critical section*/
        
        
        sem_post(&s);
        
        sem_post(&n1);
        sem_post(&n2);
        sem_post(&n3);
        sem_post(&n4);
        
        j++;
    }
    return NULL;
}


int value1,value2,value3,value4;
int OUT1,OUT2,OUT3,OUT4;
void *consumer1(void *args)
{
    while(1)
    {
        sem_wait(&n1);
        sem_wait(&s);
        
        /* critical section*/
        fp2 = fopen ("consumer activity.txt","a+");
        
        //consumer_num++;
        //printf("consumer %d entered\n",consumer_num);
        
        value1=buffer[OUT1];
        
        //citem_num++;
        //printf("consumer consumed %d item \n",citem_num);
        
        //printf("consumer consumed %d value \n",value1);
        
        fprintf(fp2,"%d\t%d\t%d\t%d\n",1,OUT1,value1);
        OUT1=(OUT1+1) % size;
        
        fclose (fp2);
        /* critical section*/
        
        sem_post(&s);
        sem_post(&e1);
    }
    return NULL;
}

void *consumer2(void *args)
{
    while(1)
    {
        sem_wait(&n2);
        sem_wait(&s);
        
        /* critical section*/
        fp2 = fopen ("consumer activity.txt","a+");
        
        //consumer_num++;
        //printf("consumer %d entered\n",consumer_num);
        
        value2=buffer[OUT2];
        
        //citem_num++;
        //printf("consumer consumed %d item \n",citem_num);
        
        //printf("consumer consumed %d value \n",value1);
        
        fprintf(fp2,"%d\t%d\t%d\t%d\n",2,OUT2,value2);
        OUT2=(OUT2+1) % size;
        
        fclose (fp2);
        /* critical section*/
        
        sem_post(&s);
        sem_post(&e2);
    }
    return NULL;
}

void *consumer3(void *args)
{
    while(1)
    {
        sem_wait(&n3);
        sem_wait(&s);
        
        /* critical section*/
        fp2 = fopen ("consumer activity.txt","a+");
        
        //consumer_num++;
        //printf("consumer %d entered\n",consumer_num);
        
        value3=buffer[OUT3];
        
        //citem_num++;
        //printf("consumer consumed %d item \n",citem_num);
        
        //printf("consumer consumed %d value \n",value1);
        
        fprintf(fp2,"%d\t%d\t%d\t%d\n",3,OUT3,value3);
        OUT3=(OUT3+1) % size;
        fclose (fp2);
        /* critical section*/
        
        sem_post(&s);
        sem_post(&e3);
    }
    return NULL;
}

void *consumer4(void *args)
{
    while(1)
    {
        sem_wait(&n4);
        sem_wait(&s);
        
        /* critical section*/
        fp2 = fopen ("consumer activity.txt","a+");
        
        //consumer_num++;
        //printf("consumer %d entered\n",consumer_num);
        
        value4=buffer[OUT4];
        
        //citem_num++;
        //printf("consumer consumed %d item \n",citem_num);
        
        //printf("consumer consumed %d value \n",value1);
        
        fprintf(fp2,"%d\t%d\t%d\t%d\n",4,OUT4,value4);
        OUT4=(OUT4+1) % size;
        
        fclose (fp2);
        /* critical section*/
        
        sem_post(&s);
        sem_post(&e4);
    }
    return NULL;
}

int main()
{
    sem_init(&s,0,1); //binary sem "access to buffer"
    
    sem_init(&n1,0,0); //count sem "# of field slots"
    sem_init(&n2,0,0);
    sem_init(&n3,0,0);
    sem_init(&n4,0,0);
    
    sem_init(&e1,0,size); //count sem "# of empty slots"
    sem_init(&e2,0,size);
    sem_init(&e3,0,size);
    sem_init(&e4,0,size);
    
    pthread_t p,c1,c2,c3,c4;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c1, NULL, consumer1, NULL);
    pthread_create(&c2, NULL, consumer2, NULL);
    pthread_create(&c3, NULL, consumer3, NULL);
    pthread_create(&c4, NULL, consumer4, NULL);
    
    pthread_join(p,NULL);
    pthread_join(c1,NULL);
    pthread_join(c2,NULL);
    pthread_join(c3,NULL);
    pthread_join(c4,NULL);
    
    sem_destroy(&s);
    
    sem_destroy(&n1);
    sem_destroy(&n2);
    sem_destroy(&n3);
    sem_destroy(&n4);
    
    sem_destroy(&e1);
    sem_destroy(&e2);
    sem_destroy(&e3);
    sem_destroy(&e4);
    return 0;
}


