#define N 5
#define M 3
#define K 2
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexTurno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexDistributore = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t aspettoRasoio = PTHREAD_COND_INITIALIZER;
pthread_cond_t aspettoScodelle = PTHREAD_COND_INITIALIZER;
pthread_cond_t dammiRasoio = PTHREAD_COND_INITIALIZER;
pthread_cond_t waitSottoUfficialeRasoi = PTHREAD_COND_INITIALIZER;
int bigliettoRasoi = 1; int bigliettoScodelle = 1;
int bigliettoDisplayRasoi = 1; int bigliettoDisplayScodelle = 1;
int numRasoi = M; int numScodelle = K; int voglioRasoio = 0;
int riconsegna = 0;

void* legionarioPari(void* arg){
	int indiceLegionario; int biglietto; indiceLegionario = *(int*) arg;
	while(1){
		pthread_mutex_lock(&mutexDistributore);
		biglietto = bigliettoRasoi;
		bigliettoRasoi++;
		pthread_mutex_unlock(&mutexDistributore);
		printf("legionario %d ho biglietto %d\n",indiceLegionario, biglietto);
		/*ora aspetta il suo turno*/
		while(bigliettoDisplayRasoi != biglietto || numRasoi == 0){
			pthread_cond_wait(&aspettoRasoio, &mutexTurno);
		}
		pthread_mutex_unlock(&mutexTurno);
		pthread_mutex_lock(&mutex); voglioRasoio = 1; pthread_cond_signal(&dammiRasoio);
		while(voglioRasoio == 1){
			pthread_cond_wait(&waitSottoUfficialeRasoi, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		printf("HO IL RASOIO. Legionario %d \n", indiceLegionario);
		sleep(3);
		pthread_mutex_lock(&mutex); riconsegna = 1; pthread_cond_signal(&dammiRasoio);
		pthread_mutex_unlock(&mutex);
	}
}

void* sottoufficialeRasoi(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(voglioRasoio == 0 && riconsegna == 0){
			pthread_cond_wait(&dammiRasoio, &mutex);
		}
		if(voglioRasoio == 1){
			printf("consegna rasoio. \n");
			numRasoi--; bigliettoDisplayRasoi++;
			voglioRasoio = 0;
			sleep(2);
			pthread_cond_broadcast(&aspettoRasoio);
			pthread_cond_broadcast(&waitSottoUfficialeRasoi);
		}
		if(riconsegna == 1){
			printf("prendo rasoio \n");
			riconsegna = 0;
			numRasoi++; pthread_cond_broadcast(&aspettoRasoio);
		}
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	pthread_t tid; int indice; int *p;  
	pthread_create(&tid, NULL, sottoufficialeRasoi, NULL);
	for(indice = 0; indice < 2; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid, NULL, legionarioPari, (void*)p);
	}
	pthread_exit(NULL);
	return 0;
}
		

