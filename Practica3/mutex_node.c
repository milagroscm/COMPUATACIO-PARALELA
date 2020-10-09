#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"


int thread_count;
struct list_node_s** head_pp;
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_rwlock_t rwlock;
pthread_mutex_t head_p_mutex;
long operaciones=100000;
double member=80;
double insert=10;
double del=10;


struct list_node_s
{
    int data;
    struct list_node_s* next;
    pthread_mutex_t mutex;
};



int Member(int value){
    
    struct list_node_s* temp_p;
    pthread_mutex_lock(&head_p_mutex);
    temp_p=*head_pp;
    while(temp_p != NULL && temp_p->data<value){
        
        if (temp_p->next != NULL){
            pthread_mutex_lock(&(temp_p->next->mutex));
        }
        if (temp_p == *head_pp){
            pthread_mutex_unlock(&head_p_mutex);
        }
        pthread_mutex_unlock(&(temp_p->mutex));
        temp_p=temp_p->next;
    }
    if (temp_p == NULL || temp_p->data >value){
        
        if (temp_p == *head_pp){
            pthread_mutex_unlock(&head_p_mutex);
        }
        if (temp_p != NULL){
            pthread_mutex_unlock(&(temp_p->mutex));
        }
        return 0;
    }
    else{
        if (temp_p == *head_pp){
            pthread_mutex_unlock(&head_p_mutex);
        }
        pthread_mutex_unlock(&(temp_p->mutex));
        return 1;
    }
}

int Insert(int value){
    struct list_node_s* curr_p= *head_pp;
    struct list_node_s* pred_p= NULL;
    struct list_node_s* temp_p;
    while(curr_p != NULL && curr_p->data<value){
        pred_p=curr_p;
        curr_p=curr_p->next;
    }
    if(curr_p == NULL || curr_p->data > value){
        temp_p=malloc(sizeof(struct list_node_s));
        temp_p->data=value;
        temp_p->next=curr_p;
        if (pred_p == NULL)
            *head_pp=temp_p;
        else
            pred_p->next=temp_p;
        return 1;
    }
    else
        return 0;
}

int Delete(int value){
    struct list_node_s* curr_p=*head_pp;
    struct list_node_s* pred_p= NULL;
    while(curr_p != NULL && curr_p->data < value){
        pred_p=curr_p;
        curr_p=curr_p->next;
    }
    if(curr_p != NULL && curr_p->data == value){
        if(pred_p == NULL)
        {
            *head_pp=curr_p->next;
            free(curr_p);
        }
        else
        {
            pred_p->next=curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else
        return 0;
}

void* MutexNode(void* r){
    long ops=(long) r;
    
    for(int j=0;j<ops*member/100;j++){
        Member(rand()%10000);
    }
    for(int j=0;j<ops*insert/100;j++){
        Insert(rand()%10000);
    }
    for(int j=0;j<ops*del/100;j++){
        Delete(rand()%10000);
    }
}

int main(int argc,char* argv[]){
    long thread;
    pthread_t* thread_handles;
    struct list_node_s* head;
    double start,end;
    head=malloc(sizeof(struct list_node_s));
    head->data=0;
    head->next=NULL;
    head_pp=&head;
    
    thread_count=strtol(argv[1],NULL,10);
    thread_handles=(pthread_t*) malloc (thread_count*sizeof(pthread_t));
    
    for(int i =0; i<1000;++i){
        Insert(i);
    }
    
    GET_TIME(start);  
    for(thread=0;thread<thread_count;thread++){
        pthread_create(&thread_handles[thread],NULL,MutexNode,(void *)thread);
    }
    for(thread=0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],NULL);
    }
    GET_TIME(end);
    printf("%.16g Mutex por nodo segundos\n", (end-start)*1000.0 );
    free(thread_handles);

    return 0;
}
