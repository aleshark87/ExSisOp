#include "printerror.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include "DBGpthread.h"

int orologioStart = 0;
int lavoro = 0;
int pronti = 0;
pthread_mutex_t mutex;
pthread_cond_t waitClock;
pthread_cond_t fineLavoro;
pthread_cond_t inizioLavoro;

void* attacchino(void* arg){
	int indiceAtt; indiceAtt = * (int*) arg;
	while(1){
		DBGpthread_mutex_lock(&mutex);
		pronti++;
		if(pronti == 1){
			DBGpthread_cond_wait(&inizioLavoro, &mutex);
		}
		else {
			DBGpthread_cond_signal(&inizioLavoro);
		}
		pronti = 0;
		DBGpthread_mutex_unlock(&mutex);
		printf("a[%d]\n", indiceAtt);
		sleep(2);
		DBGpthread_mu
		
		
	}
}

void* orologio(void* arg){
	while(1){
		sleep(1);
	}
}

int main(void){
	pthread_t tid; int *p; int indice; int rc; char msg[256];
	DBGpthread_cond_init(&waitClock, NULL);
	DBGpthread_cond_init(&fineLavoro, NULL);
	DBGpthread_cond_init(&inizioLavoro, NULL);
	DBGpthread_mutex_init(&mutex, NULL);
	for(indice = 0; indice < 2; indice++){
		p = malloc(sizeof(int)); *p = indice;
		rc = pthread_create(&tid, NULL, attacchino, (void*)p);
		sleep(3);
	}
	rc = pthread_create(&tid, NULL, orologio, NULL);
	if(rc){
		PrintERROR_andExit(rc, msg);
	}
	pthread_exit(NULL);
	return 0;
}
