#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NUM_FACHIRI 2
#define NUM_SPADE 10

int spada[NUM_SPADE];
pthread_t tid[NUM_FACHIRI];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* prendi_ucciditi(void *arg){
	int i;
	int num_fachiro;
	num_fachiro = *((int*)arg);
	while(1){
		for(i = 0; i < NUM_SPADE; i++){
			pthread_mutex_lock(& mutex);
			spada[i] = 1;
			printf("**Sono fachiro %d mi sto per trafiggere con la %d spada**\n",num_fachiro+1,i+1);
			sleep(1);
			printf("AAAAAAAAAAAAAAAAAAAAAAAAAUUUUUUUUUUUUUUUU\n");
			spada[i] = 0;
			pthread_mutex_unlock(& mutex);
			sleep(1);
		}
	}
}

int main(void){
	int i;
	int *p;
	for(i = 0; i < NUM_FACHIRI; i++){
		p = (int*) malloc(sizeof(int));
		*p = i;
		pthread_create(&tid[i], NULL, prendi_ucciditi, (void*) p);
	}
	pthread_exit(NULL);
}
	
