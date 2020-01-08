#define NUMA 5
#define NUMB 5
#define LATOA 0
#define LATOB 1
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t latoA = PTHREAD_COND_INITIALIZER;
pthread_cond_t latoB = PTHREAD_COND_INITIALIZER;

int latoAttuale = LATOA; int macchineSulPonte = 0;

void* auto_A(void* arg){
	int indiceAuto; int counter = 0; indiceAuto = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((latoAttuale == LATOB) || (macchineSulPonte >= 5)){
			pthread_cond_wait(&latoA, &mutex);
		}
		macchineSulPonte++;
		counter += macchineSulPonte;
		pthread_mutex_unlock(&mutex);
		if(counter % 10 == 0){
			printf("CADUTO NEL FOSSO. \n");
			pthread_exit(NULL);
		}
		else{
			printf("AUTO A[%d] passo sul ponte. Sul ponte ci sono %d macchine \n", indiceAuto, macchineSulPonte);
			sleep(2);
		}
		pthread_mutex_lock(&mutex);
		macchineSulPonte--; latoAttuale = LATOB; pthread_cond_broadcast(&latoB);
		pthread_mutex_unlock(&mutex);
	}
}

void* auto_B(void* arg){
	int indiceAuto; indiceAuto = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((latoAttuale == LATOA) || (macchineSulPonte >= 5)){
			pthread_cond_wait(&latoB, &mutex);
		}
		macchineSulPonte++;
		pthread_mutex_unlock(&mutex);
		printf("AUTO B[%d] passo sul ponte. Sul ponte ci sono %d macchine \n", indiceAuto, macchineSulPonte);
		sleep(2);
		pthread_mutex_lock(&mutex);
		macchineSulPonte--; latoAttuale = LATOA; pthread_cond_broadcast(&latoA);
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	pthread_t tidA[NUMA]; pthread_t tidB[NUMB]; int indice; int *p;
	for(indice = 0; indice < NUMA; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidA[indice], NULL, auto_A, (void*)p);
	}
	for(indice = 0; indice < NUMB; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidB[indice], NULL, auto_B, (void*)p);
	}
	pthread_exit(NULL);
}
