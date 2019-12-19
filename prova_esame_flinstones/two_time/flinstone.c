#define LATOA 1
#define LATOB 0
#define NUMPOSTI 2

/*library inclusion*/
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdint.h>
#include <pthread.h>  

pthread_cond_t saliti = PTHREAD_COND_INITIALIZER;
pthread_cond_t cavScesi = PTHREAD_COND_INITIALIZER;
pthread_cond_t scendere = PTHREAD_COND_INITIALIZER;
pthread_cond_t salireA = PTHREAD_COND_INITIALIZER;
pthread_cond_t salireB = PTHREAD_COND_INITIALIZER;
pthread_cond_t DinoArrivato = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int latoCavernicolo[3]={LATOA,LATOA,LATOB};
int latoDinosauro = LATOA;
int sullaCoda = 0;
int latoASalire = 0;
int latoBSalire = 0;

void *Cavernicolo(void *arg){
	int indice;
	indice = * (int*) arg;
	while(1){
		pthread_mutex_lock(&mutex);
		if(latoCavernicolo[indice] == LATOA){
			while((latoASalire == 0) || (sullaCoda == 0)){
				pthread_cond_wait(&salireA, &mutex);
			}
		}
		else {
			while((latoBSalire == 0) || (sullaCoda == 0)){
				pthread_cond_wait(&salireB, &mutex);
			}
		}
		printf("CAV[%d] sale LATO %d \n", indice, latoCavernicolo[indice]);
		fflush(stdout);
		sullaCoda --;
		if(sullaCoda == 0){
			pthread_cond_signal(&saliti);
		}
		pthread_cond_wait(&DinoArrivato, &mutex);
		sullaCoda++;
		if(sullaCoda == NUMPOSTI){
			pthread_cond_signal(&cavScesi);
		}
		if(latoCavernicolo[indice] == LATOA) latoCavernicolo[indice] = LATOB;
		if(latoCavernicolo[indice] == LATOB) latoCavernicolo[indice] = LATOA;
		printf("CAV[%d] SCESO LATO %d \n", indice, latoCavernicolo[indice]);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(4);
	}
}

void *Dinosauro(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		if(latoDinosauro == LATOA){
			printf("salite lato A\n");
			latoASalire = 1;
			pthread_cond_broadcast(&salireA);
		}
		else {
			printf("Salite lato B\n");
			latoBSalire = 1;
			pthread_cond_broadcast(&salireB);
		}
		sullaCoda = NUMPOSTI;
		printf("Dino dice : SALITE ! \n");
		fflush(stdout);
		pthread_cond_wait(&saliti, &mutex);
		latoASalire = 0;
		latoBSalire = 0;
		printf("Dino parte da Lato %d \n", latoDinosauro);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(2);
		pthread_mutex_lock(&mutex);
		if(latoDinosauro == LATOA) latoDinosauro = LATOB;
		if(latoDinosauro == LATOB) latoDinosauro = LATOA;
		printf("Dino arriva LATO %d \n",latoDinosauro);
		fflush(stdout);
		pthread_cond_broadcast(&DinoArrivato);
		printf("Dino dice SCENDETE \n");
		fflush(stdout);
		pthread_cond_wait(&cavScesi, &mutex);
		pthread_mutex_unlock(&mutex);
	}
}

int main(void){
	pthread_t tid[3];
	int i; int *p;
	for(i = 0; i < 4; i++){
		p = malloc(sizeof(int)); *p = i;
		pthread_create(&tid[i], NULL, Cavernicolo, p);
	}
	pthread_create(&tid[i], NULL, Dinosauro, NULL);
	pthread_exit(NULL);
}
