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
	/* Dichiarazione variabili */
	int i;
	int num_fachiro;
	/* cast *arg --> *int --> *int */  
	num_fachiro = *((int*)arg);
	/* loop infinito */
	while(1){
		/* per ogni spada */
		for(i = 0; i < NUM_SPADE; i++){
			pthread_mutex_lock(& mutex);
			/* prendo la spada */
			spada[i] = 1;
			printf("**Sono fachiro %d mi sto per trafiggere con la %d spada**\n",num_fachiro+1,i+1);
			/* la prendo in mano */
			sleep(1);
			printf("AAAAAAAAAAAAAAAAAAAAAAAAAUUUUUUUUUUUUUUUU\n");
			/* la poso */
			spada[i] = 0;
			pthread_mutex_unlock(& mutex);
			sleep(1);
		}
	}
}

int main(void){
	int i,rc;
	int *p;
	for(i = 0; i < NUM_FACHIRI; i++){
		p = (int*) malloc(sizeof(int));
		*p = i;
		rc = pthread_create(&tid[i], NULL, prendi_ucciditi, (void*) p);
		if(rc){
			char msg[256];
			strerror_r(rc, msg, 256);
			puts(msg);
		}
	}
	pthread_exit(NULL);
}
	
