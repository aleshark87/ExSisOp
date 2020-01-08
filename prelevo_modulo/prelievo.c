#define NUMSTUDENTI 45
#define NUMCOPIE 30

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitStudente = PTHREAD_COND_INITIALIZER;
pthread_cond_t moduliFiniti = PTHREAD_COND_INITIALIZER;
int copieFinite = 0; int stoPrendendo = 0; int numCopie = NUMCOPIE;
int hoModulo[NUMSTUDENTI];

int tuttiHannoIlModulo(){
	int indice; int check;
	for(indice = 0; indice < NUMSTUDENTI; indice++){
		if(hoModulo[indice] == 1) { check++; }
	}
	if(check == NUMSTUDENTI){
		return 1;
	}
	else{
		return 0;
	}
}
void perditaModulo(){
	int indice; 
	for(indice = 0; indice < NUMSTUDENTI; indice++){
		hoModulo[indice] = 0;
	}
}	

void* studente(void* arg){
	int indiceStudente; indiceStudente = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		while((copieFinite == 1) || (stoPrendendo == 1) || (hoModulo[indiceStudente] == 1)){
			/*printf("%d\n", indiceStudente);*/
			pthread_cond_wait(&waitStudente, &mutex);
		}
		
		stoPrendendo = 1;
		pthread_mutex_unlock(&mutex);
		printf("STUDENTE[%d] prendo una copia del modulo. \n", indiceStudente);
		sleep(1); pthread_mutex_lock(&mutex); 
		numCopie--; hoModulo[indiceStudente] = 1; stoPrendendo = 0;
		if(tuttiHannoIlModulo()){
			printf("ora tutti si sono persi il modulo... \n");
			perditaModulo();
		}
		if(numCopie == 0){
			printf("STUDENTE[%d] sono finite le copie \n", indiceStudente);
			pthread_cond_signal(&moduliFiniti);
			copieFinite = 1;
		}
		pthread_cond_broadcast(&waitStudente);
		
		pthread_mutex_unlock(&mutex);
	}
}

void* segretaria(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(copieFinite == 0){
			/*printf("wait segretaria \n");*/
			pthread_cond_wait(&moduliFiniti, &mutex);
		}
		numCopie = NUMCOPIE;
		pthread_mutex_unlock(&mutex);
		printf("SEGRETARIA dispongo le %d copie sul bancone \n", NUMCOPIE); sleep(1);
		pthread_mutex_lock(&mutex); copieFinite = 0; pthread_cond_broadcast(&waitStudente);
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	int indice; int *p; pthread_t tidStudenti[NUMSTUDENTI]; pthread_t tidSegretaria;
	for(indice = 0; indice < NUMSTUDENTI; indice++){
		hoModulo[indice] = 0;
		p = malloc(sizeof(int)); *p = indice; 
		pthread_create(&tidStudenti[indice], NULL, studente, (void*)p);
	}
	pthread_create(&tidSegretaria, NULL, segretaria, NULL);
	pthread_exit(NULL);
}
 
