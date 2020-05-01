#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_ITEMS 30
#define ITEM_LEN 8
#define N_CONSUMERS 2
#define N_PRODUCERS 4

typedef int elem;
struct block {
        elem *info;
        struct block *next;
};
typedef struct block node;
typedef struct{
	node *begin,*end, *next_produce, *next_consume;
	int free, used;
} List;


pthread_mutex_t mutex;
sem_t empty, full;

List L;

void init(List *L){
     L->begin = NULL;
     L->end = NULL;
     L->next_produce = NULL;
     L->next_consume = NULL;
     L->free = MAX_ITEMS;
     L->used = 0;
}

void create(List *L, int len){
     node *p;
     for(int i = 0; i < len; i++) {
	     p = (node*) malloc(sizeof(node));
	     if (p == NULL)
	        return;
	     p->info = NULL;
	     p->next = NULL;
	     if (L->begin == NULL)
	        L->begin = p;
	     else L->end->next = p;
	     L->end = p;
     }
     L->next_produce = L->begin;
}
int enter_item(List *L, elem *x) {
	if(L->used >= MAX_ITEMS)
		return 0;
	L->next_produce->info = x;
	L->next_produce = L->next_produce->next;
	L->used++;
	L->free--;
	return 1;
}

int remove_item(List *L, elem **r) {
	if(L->begin->info == NULL)
		return 0;
	node *p = L->begin;
	*r = p->info;

	L->begin = p->next;
	L->end->next = p;
	L->end = p;

	p->next = NULL;
	p->info = NULL;
	L->used--;
	L->free++;
	if(L->next_produce == NULL)
		L->next_produce = L->end;
	return 1;
}

int chech_empty(List *L){
	return L->begin->info == NULL;
}

int check_full(List *L){
	return L->used >= MAX_ITEMS;
}

void end(List *L){
     node *p;
     p=L->begin;
     while (p != NULL){
           L->begin=L->begin->next;
           free(p);
           p=L->begin;
     }
     L->end=NULL;
}

void imprimir(List *L){
     node *p;
     p=L->begin;
     while (p!=NULL && p->info != NULL){
           printf("%d ",*p->info);
           p=p->next;
     }
     printf("\n");
     return;
}
void produce_item(elem **x){
	*x = (int*) malloc(sizeof(int));
	**x = rand();
}

 void *producer(void *n){
 	elem *item;
 	while (1){
 		produce_item(&item);

 		sem_wait(&empty);

 		pthread_mutex_lock(&mutex);
 		enter_item(&L, item);
		printf("Producer %d produced %d\n", *((int *) n), *item);
 		pthread_mutex_unlock(&mutex);

 		sem_post(&full);
 		sleep((rand() % 20) / 10);
 	}
}

void consume_item(elem *e, int i) {
	printf("Consumer %d consumed %d\n", i, *e);
}
void *consumer(void *n){
	elem *item;
	sleep(2);
	while (1){
 		sem_wait(&full);

		pthread_mutex_lock(&mutex);
		remove_item(&L, &item);
		pthread_mutex_unlock(&mutex);

		sem_post(&empty);
		consume_item(item, *((int *) n));
 		sleep((rand() % 10) / 10);
	}
}

void test_list();


pthread_t t_producer[N_PRODUCERS], t_consumer[N_CONSUMERS];

int main(){
	int err;
	int *att;

	srand(time(NULL));
	init(&L);
	create(&L, MAX_ITEMS);

	// test_list();
	sem_init(&empty, 0, MAX_ITEMS);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex, NULL);



	for(int i = 0; i < N_CONSUMERS; i++){
		att = (int*) (int*) malloc(sizeof(int));
		*att = i + 1;
	    err = pthread_create(&(t_consumer[i]), NULL, &consumer, att);

	    if (err != 0)
	        printf("Can't create thread consumer :[%s]\n", strerror(err));
	    else    
	        printf("Thread consumer %d created\n", *att);
	}



	for(int i = 0; i < N_PRODUCERS; i++){
		att = (int*) (int*) malloc(sizeof(int));
		*att = i + 1;
	    err = pthread_create(&(t_producer[i]), NULL, &producer, att);

	    if (err != 0)
	        printf("Can't create thread producer :[%s]\n", strerror(err));
	    else    
	        printf("Thread producer %d created\n", *att);
	}


    pthread_join(t_consumer[0], NULL);
}



void test_list() {
	elem *e;
	int *x;
	int i = 0;
	do {
		produce_item(&x);
		if(enter_item(&L, x))
			printf("Produced %d\n", *x);
		else
			break;
		i++;
	} while(1);
	printf("\n");

	remove_item(&L, &e);
	printf("Consumed %d\n", *e);

	do {
		produce_item(&x);
		if(enter_item(&L, x))
			printf("Produced %d\n", *x);
		else
			break;
		i++;
	} while(1);
	printf("\n");

	while(remove_item(&L, &e))
		printf("Consumed %d\n", *e);
}
