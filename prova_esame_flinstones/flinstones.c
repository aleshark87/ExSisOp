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
pthread_cond_t discesa = PTHREAD_COND_INITIALIZER;
pthread_cond_t discesaFinita = PTHREAD_COND_INITIALIZER;
pthread_cond_t salitaA = PTHREAD_COND_INITIALIZER;
pthread_cond_t salitaB = PTHREAD_COND_INITIALIZER;
pthread_cond_t salita2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t sync = PTHREAD_COND_INITIALIZER;

int latoDinosauro = latoA;
int latoCavernicolo[numCavernicoli] = {latoA, latoA, latoB};
int cavSullaCoda = 0;


void* cavernicolo(void *arg){
	while(1){
		
		int indiceCav;
		indiceCav = * (int*) arg;
		printf("sono cavernicolo %d \n",indiceCav);
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&sync,&mutex);
		pthread_cond_signal(&discesaFinita);
		/* Qui gestisco il caso in cui il dinosauro sia di là..
		 * aspetterà la giusta richiesta di salire*/
		if(latoDinosauro != latoCavernicolo[indiceCav]){
			if(latoCavernicolo == latoA){
				pthread_cond_wait(&salitaA,&mutex);
			}
			else{
				pthread_cond_wait(&salitaB,&mutex);
			}
		}
		if(latoCavernicolo == latoA){
			pthread_cond_wait(&salitaA,&mutex);
		}
		else{
			pthread_cond_wait(&salitaB,&mutex);
		}
		printf("cav %d SALGO \n",indiceCav);
		
		cavSullaCoda++;
		if(cavSullaCoda == 2){
			pthread_cond_broadcast(&salita2);
		}
		else{
			pthread_cond_wait(&salita2,&mutex);
		}
		if(latoCavernicolo[indiceCav] == latoA){
			latoCavernicolo[indiceCav] = latoB;
		}
		else{
			latoCavernicolo[indiceCav] = latoA;
		}
		/* gestione discesa */
		pthread_cond_wait(&discesa,&mutex);
		cavSullaCoda--;
		if(cavSullaCoda != 0){
			pthread_cond_wait(&discesaFinita,&mutex);
		}
		pthread_cond_broadcast(&discesaFinita);
		pthread_mutex_unlock(&mutex);
		sleep(tempoGiretto);
	}
	
}

void* dinosauro(void *arg){
	while(1){
		int i;
		pthread_mutex_lock(&mutex);
		/* prima di iniziare a far salire cavernicoli,
		 * devo prima far scendere */
		 /* SCENDETE ! */
		printf("DOVETE SCENDERE !\n");
		pthread_cond_broadcast(&discesa);
		printf("ASPETTO CHE SIATE SCESI TUTTI\n");
		pthread_cond_signal(&sync);
		pthread_cond_wait(&discesaFinita,&mutex);
		printf("INIZIO LA PROCEDURA PER TRAGHETTARVI\n");
		printf("%d\n",latoDinosauro);
		if(latoDinosauro == latoA){
			printf("DOVETE SALIRE !\n");
			pthread_cond_broadcast(&salitaA);
			printf("ASPETTO CHE SALIATE IN DUE DA A\n");
			for(i = 0; i <  numCavernicoli; i++){
				printf("cav %d = %d\n",i,latoCavernicolo[i]);
			}
			pthread_cond_wait(&salita2,&mutex);
			printf("ORA SISTEMO LE VARIABILI E LASCIO LA MUTEX\n");
			latoDinosauro = latoB;
			printf("%d\n",latoDinosauro);
			/* la variabile latoCavernicolo, cavSullaCoda vanno sistemate
			 * in base all'indice nella procedura relativa.*/
			 pthread_mutex_unlock(&mutex);
		}
		if(latoDinosauro == latoB){
			printf("DOVETE SALIRE !\n");
			pthread_cond_broadcast(&salitaB);
			printf("ASPETTO CHE SALIATE IN DUE DA B\n");
			pthread_cond_wait(&salita2,&mutex);
			printf("ORA SISTEMO LE VARIABILI E LASCIO LA MUTEX\n");
			latoDinosauro = latoA;
			pthread_mutex_unlock(&mutex);
		}
		sleep(tempoTrasporto);
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
	
	
	
	

