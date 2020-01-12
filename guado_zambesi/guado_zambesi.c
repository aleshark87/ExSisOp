#define NUMIPPO 2
#include "printerror.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include "DBGpthread.h"


pthread_mutex_t mutex;
pthread_cond_t waitIppo; 
pthread_cond_t waitDiscesa;
pthread_cond_t waitLemure;
pthread_cond_t siamoScesi;
int salire[NUMIPPO] = {0,0}; int scendere[NUMIPPO] = {0,0}; int attraversamento = 0;
int saliti[NUMIPPO] = {0,0};
void* ippopotamo(void* arg){
	int indiceIppo; indiceIppo = *(int*)arg;
	while(1){
		DBGpthread_mutex_lock(&mutex);
		while(attraversamento == 1){
			DBGpthread_cond_wait(&waitIppo, &mutex);
		}
		printf("IPPO[%d], SALIRE !! \n", indiceIppo);
		salire[indiceIppo] = 1; pthread_cond_broadcast(&waitLemure);
		DBGpthread_mutex_unlock(&mutex);
		sleep(2);
		printf("IPPO[%d], SI PARTE !!. \n", indiceIppo);
		DBGpthread_mutex_lock(&mutex);
		salire[indiceIppo] = 0;
		attraversamento = 1; DBGpthread_mutex_unlock(&mutex);
		sleep(3);
		printf("IPPO[%d], SIAMO ARRIVATI !!.\n", indiceIppo);
		DBGpthread_mutex_lock(&mutex); attraversamento = 0;
		scendere[indiceIppo] = 1;  DBGpthread_cond_broadcast(&waitDiscesa);
		while(saliti[indiceIppo] > 0){
			DBGpthread_cond_wait(&siamoScesi, &mutex);
		}
		scendere[indiceIppo] = 0; saliti[indiceIppo] = 0;
		DBGpthread_mutex_unlock(&mutex);
		printf("FUCKING TROMBA D'ARIA !!!!!!!!!!!! \n"); sleep(3);
	}
}

void* lemure(void* arg){
	int indiceLemure; indiceLemure = *(int*)arg;
	while(1){
		DBGpthread_mutex_lock(&mutex);
		while(salire == 0 || saliti > 4){
			DBGpthread_cond_wait(&waitLemure, &mutex);
		}
		printf("LEMURE[%d], saliti. \n",indiceLemure);
		saliti++; 
		while(scendere == 0){
			DBGpthread_cond_wait(&waitDiscesa, &mutex);
		}
		saliti--; printf("LEMURE[%d], scesi. \n",indiceLemure);
		if(saliti == 0){
			DBGpthread_cond_broadcast(&siamoScesi);
		}
		DBGpthread_mutex_unlock(&mutex);
	}
}
		
int main(void){
	pthread_t tid; int indice; int *p; int rc; char msg[256];
	DBGpthread_mutex_init(&mutex);
	DBGpthread_cond_init(&waitIppo);
	DBGpthread_cond_init(&waitLemure);
	DBGpthread_cond_init(&waitDiscesa);
	DBGpthread_cond_init(&siamoScesi);
	for(indice = 0; indice < 2; indice++){
		saliti[indice] = 0;
	}
	for(indice = 0; indice < 2; indice++){
		p = malloc(sizeof(int)); *p = indice;
		rc = pthread_create(&tid, NULL, ippopotamo, (void*)p);
		if(rc){
			PrintERROR_andExit(rc, msg);
		}
	}
	for(indice = 0; indice < 4 ; indice++){
		p = malloc(sizeof(int)); *p = indice;
		rc = pthread_create(&tid, NULL, lemure, (void*)p);
		if(rc){
			PrintERROR_andExit(rc, msg);
		}
	}
	pthread_exit(NULL);
	return 0;
}
