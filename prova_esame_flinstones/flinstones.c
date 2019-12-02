#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define latoA 0
#define latoB 1 
#define tempoTrasporto 2 
#define tempoGiretto 4 
#define numCavernicoli 3 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t aspetta = PTHREAD_COND_INITIALIZER;
pthread_cond_t possosalireB = PTHREAD_COND_INITIALIZER;
pthread_cond_t lato = PTHREAD_COND_INITIALIZER;
pthread_cond_t latoB1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t svegliaDino = PTHREAD_COND_INITIALIZER;


int latoDinosauro = latoA;
int latoCavernicolo[numCavernicoli] = {latoA, latoA, latoB};
int cavLatoA = 0;
int cavLatoB = 0;



void* cavernicolo(void *arg){
	while(1){
		int indiceCav;
		int i;
		indiceCav = * (int*) arg;
		printf("sono cavernicolo %d \n",indiceCav);
		pthread_mutex_lock(&mutex);
		printf("lato din = %d ",latoDinosauro);
		if(latoDinosauro == latoCavernicolo[indiceCav]){
			cavLatoA++;
			if(latoDinosauro == latoA) latoCavernicolo[indiceCav] = latoB;
			if(latoDinosauro == latoB) latoCavernicolo[indiceCav] = latoA; 
			if(cavLatoA == 2){
				printf("SIAMO IN DUE SVEGLIA DINOSAURO, io sono %d \n",indiceCav);
				pthread_cond_signal(&svegliaDino);
				for(i = 0; i < numCavernicoli; i++){
					printf("lato %d = %d\n",i,latoCavernicolo[i]);
				}
			}
			else{
				printf("aspetto un altro cavernicolo per andare \n");
				pthread_cond_wait(&aspetta,&mutex);
			}
		}
		
		pthread_mutex_unlock(&mutex);
		sleep(tempoGiretto);
	}
	
}

void* dinosauro(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		if(latoDinosauro == latoA){
			pthread_cond_wait(&svegliaDino, &mutex);
			pthread_cond_broadcast(&aspetta);
			printf("SI PARTE \n");
			sleep(tempoTrasporto);
			latoDinosauro = latoB;
		}
		if(latoDinosauro == latoB){
			pthread_cond_wait(&svegliaDino, &mutex);
			printf("SI PARTE \n");
			sleep(tempoTrasporto);
			latoDinosauro = latoA;
		}
		pthread_mutex_unlock(&mutex);
	}
	
}

int main(void){
	pthread_t tid;
	int i;
	for(i = 0; i < numCavernicoli; i++){
		int *p;
		p = (int*)malloc(sizeof(int));
		*p = i;
		pthread_create(&tid, NULL, cavernicolo, (void*)p);
	}
	pthread_create(&tid, NULL, dinosauro, NULL);
	pthread_exit(NULL);
	return 0;
}
	
	
	
	

