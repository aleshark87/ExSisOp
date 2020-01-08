#define NUMS1 2
#define NUMS2 4
#define NUMS3 2
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t s1Libero = PTHREAD_COND_INITIALIZER;
pthread_cond_t s2Libero = PTHREAD_COND_INITIALIZER;
pthread_cond_t s3Libero = PTHREAD_COND_INITIALIZER;

int s1[NUMS1];
int s2[NUMS2];
int s3[NUMS3];

/* Ritorna 1 se trova uno strumento S1 libero, altrimenti ritorna -1 */
int cerco_prendoS1(){
	int indice;
	for(indice = 0; indice < NUMS1; indice++){
		if(s1[indice] == 1){
			/*Ho trovato uno strumento 1 libero..*/
			s1[indice] = 0; /*lo prendo*/
			return indice;
		}
	}
	return -1;
} 

int cerco_prendoS2(){
	int indice;
	for(indice = 0; indice < NUMS2; indice++){
		if(s2[indice] == 1){
			/*Ho trovato uno strumento 1 libero..*/
			s2[indice] = 0; /*lo prendo*/
			return indice;
		}
	}
	return -1;
} 
int cerco_prendoS3(){
	int indice;
	for(indice = 0; indice < NUMS3; indice++){
		if(s3[indice] == 1){
			/*Ho trovato uno strumento 1 libero..*/
			s3[indice] = 0; /*lo prendo*/
			return indice;
		}
	}
	return -1;
} 

void restituisco_S1(int indice){
	int i;
	for(i = 0; i < NUMS1; i++){
		if(i == indice){
			/*lo strumento è nuovamente disponibile*/
			s1[indice] = 1;
			printf("BROADCAST S1 \n");
			pthread_cond_broadcast(&s1Libero);
			return;
		}
	}
	printf("ERROR \n");
}
void restituisco_S2(int indice){
	int i;
	for(i = 0; i < NUMS2; i++){
		if(i == indice){
			/*lo strumento è nuovamente disponibile*/
			s2[indice] = 1;
			pthread_cond_broadcast(&s2Libero);
			return;
		}
	}
	printf("ERROR \n");
}
void restituisco_S3(int indice){
	int i;
	for(i = 0; i < NUMS3; i++){
		if(i == indice){
			/*lo strumento è nuovamente disponibile*/
			s3[indice] = 1;
			pthread_cond_broadcast(&s3Libero);
			return;
		}
	}
	printf("ERROR \n");
}

void superStampa(){
	int indice;
	for(indice = 0; indice < 2 ; indice++){
		printf("*******************************************************\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞⅞\n");
	}
}

void* lavorazione1(void* arg){
	int indiceS1; int indiceS2; int indiceS3; int indiceLavorazione = *(int*) arg;
	int numLavorazione = 0;
	while(1){
		numLavorazione++;
		pthread_mutex_lock(&mutex);
		printf("Sono L1[%d], chiedo gli strumenti \n", indiceLavorazione);
		/* QUESTO SECONDO ME PUÒ ESSERE UN GRANDE CICLO WHILE */
		indiceS1 = cerco_prendoS1();
		while(indiceS1 == -1){
			pthread_cond_wait(&s1Libero, &mutex);
			indiceS1 = cerco_prendoS1();
		}
		/*printf("L1 ho lo strumento s1[%d] \n", indiceS1 );*/
		indiceS2 = cerco_prendoS2();
		while(indiceS2 == -1){
			pthread_cond_wait(&s2Libero, &mutex);
			indiceS2 = cerco_prendoS2();
		}
		/*printf("L1 ho lo strumento s2[%d] \n", indiceS2 );*/
		indiceS3 = cerco_prendoS3();
		while(indiceS3 == -1){
			pthread_cond_wait(&s3Libero, &mutex);
			indiceS3 = cerco_prendoS3();
		}
		/*printf("L1 ho lo strumento s3[%d] \n", indiceS3 );*/
		pthread_mutex_unlock(&mutex);
		printf("L1 ora possiamo iniziare a lavorare \n");
		sleep(10);
		printf("L1 ora dobbiamo restituire gli strumenti \n");
		pthread_mutex_lock(&mutex);
		restituisco_S1(indiceS1); restituisco_S2(indiceS2); restituisco_S3(indiceS3);
		pthread_mutex_unlock(&mutex);
		superStampa();
		printf("L1[%d] Numero Lavorazione %d \n", indiceLavorazione, numLavorazione);
		superStampa();
	}
}	

void* lavorazione2(void* arg){
	int indiceS1; int indiceS2; int indiceS3; int indiceLavorazione = * (int*) arg;
	int numLavorazioni = 0; 
	while(1){
		numLavorazioni += 1;
		pthread_mutex_lock(&mutex);
		indiceS1 = cerco_prendoS1();
		while(indiceS1 == -1){
			printf("Sono l2[%d] non ho trovato strumenti 1. Vado in wait. \n", indiceLavorazione);
			fflush(stdout);
			pthread_cond_wait(&s1Libero, &mutex);
			indiceS1 = cerco_prendoS1();
		}
		printf("Sono l2[%d] ho lo strumento s1[%d] \n", indiceLavorazione, indiceS1 );
		indiceS2 = cerco_prendoS2();
		while(indiceS2 == -1){
			pthread_cond_wait(&s2Libero, &mutex);
			indiceS2 = cerco_prendoS2();
		}
		printf("Sono l2[%d] ho lo strumento s2[%d] \n",indiceLavorazione, indiceS2 );
		indiceS3 = cerco_prendoS3();
		while(indiceS3 == -1){
			pthread_cond_wait(&s3Libero, &mutex);
			indiceS3 = cerco_prendoS3();
		}
		printf("L2[%d] ho lo strumento s3[%d] ", indiceLavorazione, indiceS3 );
		printf("Sono L2[%d] INIZIO A LAVORARE.. \n", indiceLavorazione);
		pthread_mutex_unlock(&mutex);
		sleep(8);
		superStampa();
		printf("L2[%d] FINITO IL MIO LAVORO[%d] \n", indiceLavorazione, numLavorazioni);
		superStampa();
		pthread_mutex_lock(&mutex);
		restituisco_S1(indiceS1); restituisco_S2(indiceS2); restituisco_S3(indiceS3);
		pthread_mutex_unlock(&mutex);
		
	}
}	

int main(void){
	int indice; pthread_t tid1[2];  int *p;
	for(indice = 0; indice < NUMS1; indice++){
		s1[indice] = 1;
	}
	for(indice = 0; indice < NUMS2; indice++){
		s2[indice] = 1;
	}
	for(indice = 0; indice < NUMS3; indice++){
		s3[indice] = 1;
	}
	for(indice = 0; indice < 2; indice++){
		p = malloc(sizeof(int)); *p = indice;
		pthread_create(&tid1[indice], NULL, lavorazione1, (void*)p);
		sleep(1);
		pthread_create(&tid1[indice], NULL, lavorazione2, (void*)p);
		
	}
	pthread_exit(NULL);
}
