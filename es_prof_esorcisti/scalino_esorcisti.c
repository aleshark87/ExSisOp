#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t fineIntervento = PTHREAD_COND_INITIALIZER;
int stoBene = 1;
int aiutantiArrivati = 0;

void* professore(void* arg){
	while(1){
		printf("INCIAMPO NELLA SCALA ");
		printf("PORCO ***, PORCA ******* \n");
		pthread_mutex_lock(&mutex);
		printf("chiamo i soccorritori \n");
		stoBene = 0; 
		pthread_cond_broadcast(&cond);
		
		while(aiutantiArrivati < 2) { 
			pthread_cond_wait(&cond2, &mutex); 
		}
		printf("mi curano \n");
		sleep(2);
		aiutantiArrivati = 0; pthread_cond_broadcast(&fineIntervento);
		stoBene = 1;
		pthread_mutex_unlock(&mutex);
		printf("FACCIO LEZIONE \n");
		sleep(4);
	}
}

void* medico(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(stoBene == 1){
			printf("ASPETTO \n");
			pthread_cond_wait(&cond, &mutex);
		}
		aiutantiArrivati++;
		if(aiutantiArrivati == 2){
			pthread_cond_signal(&cond2);
		}
		printf("sono medico MUHAHAHAHHA \n");
		pthread_cond_wait(&fineIntervento, &mutex);
		pthread_mutex_unlock(&mutex);
	}
}

void* esorcista(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(stoBene == 1){
			printf("ASPETTO \n");
			pthread_cond_wait(&cond, &mutex);
		}
		aiutantiArrivati++;
		if(aiutantiArrivati == 2){
			pthread_cond_signal(&cond2);
		}
		printf("sono esorcista AHAHAHAHAHAHHA \n");
		pthread_cond_wait(&fineIntervento, &mutex);
		pthread_mutex_unlock(&mutex);
	}
}


int main(void){
	pthread_t pid; 
	pthread_create(&pid, NULL, professore, NULL);
	pthread_create(&pid, NULL, medico, NULL);
	pthread_create(&pid, NULL, esorcista, NULL);
	pthread_exit(NULL);
}
		
