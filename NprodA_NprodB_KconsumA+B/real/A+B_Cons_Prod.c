/*defining macros*/
#define TIPO_A 0
#define TIPO_B 1
#define numPRODa 3
#define numPRODb 5
#define numCONS 10
#define SLEEP_TIME 1

/*library inclusion*/
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h> 

uint64_t datoA = 0;
uint64_t datoB = 0; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condWaitA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWaitB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condConsB = PTHREAD_COND_INITIALIZER;
pthread_cond_t aspettoBro = PTHREAD_COND_INITIALIZER;
int bufferPienoA = 0;
int bufferPienoB = 0;
int glieLodicoio = 0;

void* produttoreA(void *arg){
	while(1){
		pthread_mutex_lock(& mutex);
		if(bufferPienoA){
			pthread_cond_wait(& condWaitA, & mutex);
		}
		sleep(1);
		printf("PRODOTTO DATO A \n");
		datoA++;
		bufferPienoA = 1;
		if(bufferPienoB == 0) {
			 pthread_cond_wait(& aspettoBro, & mutex);
		}
		else {
			pthread_cond_signal(& aspettoBro);
		}
		pthread_cond_signal(& condConsA); 
		pthread_mutex_unlock(& mutex);
	}
}

void* produttoreB(void *arg){
	while(1){
		pthread_mutex_lock(& mutex);
		if(bufferPienoB){
			pthread_cond_wait(& condWaitB, & mutex);
		}
		sleep(1);
		printf("PRODOTTO DATO B \n");
		datoB++;
		bufferPienoB = 1;
		if(bufferPienoA == 0) {
			 pthread_cond_wait(& aspettoBro, & mutex);
		}
		else {
			pthread_cond_signal(& aspettoBro);
		}
		pthread_cond_signal(& condConsB);
		pthread_mutex_unlock(& mutex);
	}
}
/*IL MIO PROBLEMA ATTUALE E' CHE I THREAD DEVONO ESSERE SICRONIZZATI
 * PER MANDARE IL SEGNALE AL CONSUMATORE.
 * FARO' SI CHE ASPETTINO PRIMA IL COMPAGNO */

void* consumatoreAB(void *arg){
	while(1){
		pthread_mutex_lock(& mutex);
		pthread_cond_wait(&condConsA, & mutex);
		pthread_cond_wait(&condConsB, & mutex);
		sleep(1);
		printf("CONSUMO DATO A e B \n");
		datoA--;
		datoB--;
		bufferPienoA = 0;
		bufferPienoB = 0;
		pthread_cond_signal(& condWaitA);
		pthread_cond_signal(& condWaitB);
		pthread_mutex_unlock(& mutex);
	}
}

int main(void){
	pthread_t tid;
	int i;
	for(i = 0; i < numPRODa; i++){
		pthread_create(& tid , NULL, produttoreA, NULL);
	}
	for(i = 0; i < numPRODb; i++){
		pthread_create(& tid, NULL, produttoreB, NULL);
	}
	for(i = 0; i < numCONS; i++){
		pthread_create(& tid, NULL, consumatoreAB, NULL);
	}
	pthread_exit(NULL);
}


