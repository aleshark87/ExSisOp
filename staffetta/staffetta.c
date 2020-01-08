#define NUMALFA 5
#define NUMBETA 4
#define ALFA 0
#define BETA 1
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitA = PTHREAD_COND_INITIALIZER;
pthread_cond_t waitB = PTHREAD_COND_INITIALIZER;
int numGiriA[NUMALFA]; int numGiriB[NUMBETA]; int turno = ALFA; 
int pistaOccupata = 0;

/*ritorna 1 se l'atleta indice non può prendere il testimone */
/*ritorna 0 se l'atleta indice può prendere il testimone */
int shallNotPass(int indice, int squadra){
	int i; int check = 0;
	if(squadra == ALFA){
		for(i = 0; i < indice; i++){
			if(numGiriA[indice] > (numGiriA[i] + 1)){
				 check += 1;
			}
		}
		for(i = (indice + 1); i < NUMALFA; i++){
			if(numGiriA[indice] > (numGiriA[i] + 1)){
				check += 1;
			}
		}
	}
	else {
		for(i = 0; i < indice; i++){
			if(numGiriB[indice] > (numGiriB[i] + 1)){
				 check += 1;
			}
		}
		for(i = (indice + 1); i < NUMBETA; i++){
			if(numGiriB[indice] > (numGiriB[i] + 1)){
				check += 1;
			}
		}
	}
	/* se check è 0. Può prendere il testimone, altrimenti no. */
	if(check == 0){
		/*printf("ATLETA SQUADRA %d[%d] PUO PRENDERE IL TESTIMONE. CHECK = %d \n", squadra, indice, check);*/
		return 0;
	}
	else {
		return 1;
	}
}

void* atletaALFA(void* arg){
	int indiceAtleta; indiceAtleta = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((shallNotPass(indiceAtleta, ALFA)) || (turno == BETA) || (pistaOccupata == 1)){
			pthread_cond_wait(&waitA, &mutex);
		}
		pistaOccupata = 1;
		pthread_mutex_unlock(&mutex);
		printf("ATLETA A %d inizia il GIRO, numero Giri = %d \n", indiceAtleta, numGiriA[indiceAtleta]);
		sleep(1);
		printf("PASSO TESTIMONE A SQUADRA BETA \n");
		pthread_mutex_lock(&mutex); numGiriA[indiceAtleta]++; turno = BETA; pistaOccupata = 0;
		pthread_cond_broadcast(&waitB); pthread_mutex_unlock(&mutex);
	}
}

void* atletaBETA(void* arg){
	int indiceAtleta; indiceAtleta = *(int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((shallNotPass(indiceAtleta, BETA)) || (turno == ALFA) || (pistaOccupata == 1)){
			pthread_cond_wait(&waitB, &mutex);
		}
		pistaOccupata = 1;
		pthread_mutex_unlock(&mutex);
		printf("ATLETA B %d inizia il GIRO , numero Giri = %d \n", indiceAtleta, numGiriB[indiceAtleta]);
		sleep(1);
		printf("PASSO TESTIMONE A SQUADRA ALFA \n");
		pthread_mutex_lock(&mutex); numGiriB[indiceAtleta]++; turno = ALFA; pistaOccupata = 0;
		pthread_cond_broadcast(&waitA); pthread_mutex_unlock(&mutex);
	}
}
		

int main(void){
	int indice; pthread_t tidA[NUMALFA]; pthread_t tidB[NUMBETA]; int *p;
	for(indice = 0; indice < NUMALFA; indice++){
		numGiriA[indice] = 0;
	}
	for(indice = 0; indice < NUMBETA; indice++){
		numGiriB[indice] = 0;
	}
	for(indice = 0; indice < NUMALFA; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidA[indice], NULL, atletaALFA, (void*)p);
	}
	for(indice = 0; indice < NUMBETA; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tidB[indice], NULL, atletaBETA, (void*)p);
	}
	pthread_exit(NULL);
}
