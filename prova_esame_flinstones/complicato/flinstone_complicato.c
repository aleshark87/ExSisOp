#define NUMCAV 3
#define LATOA 0
#define LATOB 1
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t wait = PTHREAD_COND_INITIALIZER;
pthread_cond_t partenza = PTHREAD_COND_INITIALIZER;
pthread_cond_t arrivo = PTHREAD_COND_INITIALIZER;
int sullaCoda = 0; int latoCavernicolo[NUMCAV]; int latoDino = LATOA;
int voglioPartire = 1;

void* cavernicolo(void* arg){
	int indiceCav; indiceCav = *(int*)arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((sullaCoda >= 2) || (latoDino != latoCavernicolo[indiceCav])
				|| (voglioPartire == 0)){
			pthread_cond_wait(&wait, &mutex); 
		}
		sullaCoda++; 
		printf("CAV[%d] SONO SALITO LATO %d !!\n",indiceCav, latoCavernicolo[indiceCav]);
		if(sullaCoda == 2){
			pthread_cond_signal(&partenza);
		}
		pthread_cond_wait(&arrivo,&mutex);
		if(latoCavernicolo[indiceCav] == LATOA){
			 latoCavernicolo[indiceCav] = LATOB;
		}
		else {
			latoCavernicolo[indiceCav] = LATOA;
		}
		pthread_mutex_unlock(&mutex);
		printf("CAV[%d] SONO ARRIVATO LATO %d E FACCIO UN GIRETTO.\n", indiceCav, latoCavernicolo[indiceCav]);
		sleep(4);
	}
}

void* dinosauro(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		voglioPartire = 1;
		while(sullaCoda < 2) { 
			pthread_cond_wait(&partenza, &mutex); 
		}
		voglioPartire = 0;
		pthread_mutex_unlock(&mutex);
		printf("ora si parte, DINO \n");
		sleep(2);
		pthread_mutex_lock(&mutex); 
		if(latoDino == LATOA){
			latoDino = LATOB;
		}
		else {
			latoDino = LATOA;
		}
		sullaCoda = 0;
		printf("sono arrivato, DINO \n");
		pthread_cond_broadcast(&arrivo); pthread_cond_broadcast(&wait);
		pthread_mutex_unlock(&mutex);
	}
}
int main(void){
	pthread_t tidCav[NUMCAV]; pthread_t tidDino; int indice; int *p;
	latoCavernicolo[0] = LATOA; latoCavernicolo[1] = LATOB; latoCavernicolo[2] = LATOA;
	for(indice = 0; indice < NUMCAV; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidCav[indice], NULL, cavernicolo, (void*)p);
	}
	pthread_create(&tidDino, NULL, dinosauro, NULL);
	pthread_exit(NULL);
}
