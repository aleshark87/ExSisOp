#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


#define NUM_BANCHE 3
pthread_t tid[NUM_BANCHE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int denaro[NUM_BANCHE];
int operazioni[NUM_BANCHE];
int critical = 0;

void* deposita(void *arg){
	int indice;
	printf("thread deposita\n");
	indice = *((int*)arg);
	free(arg);
	while(1){
		struct timespec ts={0, 100000000L }; /* 1/10 sec */
		sleep(1);
		pthread_mutex_lock(& mutex);
		critical = 1;
		operazioni[indice] += 10;
		denaro[indice] += 5;
		printf("aggiungo 10 su OP\n");
		nanosleep(&ts,NULL);
		pthread_mutex_unlock(& mutex);
		critical = 0;
		
	}
	pthread_exit(NULL);
}

void* preleva(void *arg){
	int indice;
	printf("thread preleva\n");
	indice = *((int*)arg);
	free(arg);
	while(1){
		struct timespec ts={0, 100000000L }; /* 1/10 sec */
		sleep(1);
		pthread_mutex_lock(& mutex);
		critical = 1;
		operazioni[indice] -= 9;
		denaro[indice] -= 4;
		printf("tolgo nove da OP \n");
		nanosleep(&ts,NULL);
		pthread_mutex_unlock(& mutex);
		critical = 0;
	}
	pthread_exit(NULL);
}

void* banca_italia(void *arg){
	int i,somma_denaro,somma_operazioni;
	while(1){
		pthread_mutex_lock(& mutex);
		for(i = 0; i < NUM_BANCHE; i++){
			somma_denaro += denaro[i];
			somma_operazioni += operazioni[i];
			printf("BANCA %d , DENARO: %d , OPERAZIONI: %d \n",i,somma_denaro,somma_operazioni);
		}
		sleep(3);
		pthread_mutex_unlock(& mutex);
		sleep(10);
	}
}

int main(void){
	int *p;
	int i,rc,k;
	char msg[256];
	for(i = 0; i < NUM_BANCHE; i++){
		for(k=0; k < 5; k++){
			p = (int*)malloc(sizeof(int));
			*p = i;
			rc = pthread_create(& tid[i], NULL, deposita,(void*)p);
			if(rc){
				strerror_r(rc, msg, 256);
				puts(msg);
			}
		}
		for(k=0; k < 4; k++){
			p = (int*)malloc(sizeof(int));
			*p = i;
			rc = pthread_create(& tid[i], NULL, preleva,(void*)p);
			if(rc){
				strerror_r(rc, msg, 256);
				puts(msg);
			}
		}
		rc = pthread_create(& tid[i], NULL, banca_italia, NULL);
		if(rc){
			strerror_r(rc, msg, 256);
			puts(msg);
		}
	}
	pthread_exit(NULL);
}
		 
