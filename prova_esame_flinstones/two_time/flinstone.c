#define NUMCAVERNICOLI 3
#define NUMDINOSAURI 1
#define LATOA 1
#define LATOB 0
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int latoCavernicolo[NUMCAVERNICOLI] = {LATOA, LATOA, LATOB};
int sullaCoda = 0; int latoDino = LATOA;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t latoA = PTHREAD_COND_INITIALIZER;
pthread_cond_t latoB = PTHREAD_COND_INITIALIZER;
pthread_cond_t parti = PTHREAD_COND_INITIALIZER;
pthread_cond_t arrivati = PTHREAD_COND_INITIALIZER;
pthread_cond_t scesi = PTHREAD_COND_INITIALIZER;

void* cavernicolo(void * arg){
	int indiceCav; int numeroTrag = 0; indiceCav = *(int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((sullaCoda >= 2) || (latoDino != latoCavernicolo[indiceCav])){
			if(latoCavernicolo[indiceCav] == LATOA) pthread_cond_wait(&latoA, &mutex);
			if(latoCavernicolo[indiceCav] == LATOB) pthread_cond_wait(&latoB, &mutex);
		}
		/* Suppongo che a questo punto, il Cavernicolo sia dal lato giusto */
		printf("CAV[%d] SALGO !! \n", indiceCav);
		sullaCoda++;
		if(sullaCoda >= 2) pthread_cond_signal(&parti);
		/*printf("cav[%d] WAITO \n", indiceCav);*/
		pthread_cond_wait(&arrivati, &mutex);
		if(latoCavernicolo[indiceCav] == LATOA){
			 latoCavernicolo[indiceCav] = LATOB;
		}
		else latoCavernicolo[indiceCav] = LATOA;
		sullaCoda--;
		if(sullaCoda == 0) pthread_cond_signal(&scesi);
		pthread_mutex_unlock(&mutex);
		numeroTrag++;
		printf("NUMERO TRAGHETTAMENTI CAV[%d] = %d \n", indiceCav, numeroTrag);
		printf("CAV[%d] SCESO !!, mi faccio un giro \n", indiceCav);
		sleep(6);
		
	}
}

void* dinosauro(void * arg){
	while(1){
		pthread_mutex_lock(&mutex);
		if(latoDino == LATOA) pthread_cond_signal(&latoA);
		if(latoDino == LATOB) pthread_cond_signal(&latoB);
		while(sullaCoda < 2) {
			pthread_cond_wait(&parti, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		printf("DINOSAURO: siamo pronti per partire dal lato %d\n", latoDino);
		sleep(4);
		pthread_mutex_lock(&mutex);
		pthread_cond_broadcast(&arrivati);
		if(latoDino == LATOA){
			 latoDino = LATOB;
		}
		else latoDino = LATOA;
		while(sullaCoda != 0){
			pthread_cond_wait(&scesi, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		printf("DINOSAURO:  siamo arrivati sul lato %d \n", latoDino);
	}
}

int main(void){
	pthread_t tid[NUMCAVERNICOLI + NUMDINOSAURI]; int indice; int *p;
	for(indice = 0; indice < NUMCAVERNICOLI; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid[indice], NULL, cavernicolo, (void*)p);
	}
	for(indice = NUMCAVERNICOLI; indice < (NUMCAVERNICOLI + NUMDINOSAURI);
		indice++){
			p = malloc(sizeof(int)); *p = indice;
			pthread_create(&tid[indice], NULL, dinosauro, (void*)p);
	}
	pthread_exit(NULL);
}
