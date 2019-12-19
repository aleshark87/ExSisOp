#define NUMS1 2
#define NUMS2 4
#define NUMS3 2
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutexS1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexS2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexS3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t s1Libero = PTHREAD_COND_INITIALIZER;
pthread_cond_t s2Libero = PTHREAD_COND_INITIALIZER;
pthread_cond_t s3Libero = PTHREAD_COND_INITIALIZER;

int s1[NUMS1];
int s2[NUMS2];
int s3[NUMS3];

int s1liberi(){
	int indice;
	pthread_mutex_lock(&mutexS1);
	for(indice = 0; indice < NUMS1; indice++){
		if(s1[indice] == 1){
			pthread_mutex_unlock(&mutexS1);
			return indice;
		}
	}
	pthread_mutex_unlock(&mutexS1);
	return -1;
}

int s2liberi(){
	int indice;
	pthread_mutex_lock(&mutexS2);
	for(indice = 0; indice < NUMS2; indice++){
		if(s2[indice] == 1){
			pthread_mutex_unlock(&mutexS2);
			return indice;
		}
	}
	pthread_mutex_unlock(&mutexS2);
	return -1;
}

int s3liberi(){
	int indice;
	pthread_mutex_lock(&mutexS3);
	for(indice = 0; indice < NUMS3; indice++){
		if(s3[indice] == 1){
			pthread_mutex_unlock(&mutexS3);
			return indice;
		}
	}
	pthread_mutex_unlock(&mutexS3);
	return -1;
}

void* lavorazione1(void* arg){
	int indiceS1; int indiceS2; int indiceS3;
	while(1){
		printf("inizio lavorazione 1... \n");
		printf("prendo gli strumenti necessari.\n");
		indiceS1 = s1liberi();
		if(indiceS1 != -1){
			pthread_mutex_lock(&mutexS1);
			s1[indiceS1] = 0;
			pthread_mutex_unlock(&mutexS1);
		}
		else {
			printf("aspetto che si liberi un s1 [L1]\n ");
			pthread_mutex_lock(&mutexS1);
			pthread_cond_wait(&s1Libero, &mutexS1);
			pthread_mutex_unlock(&mutexS1);
			indiceS1 = s1liberi();
			pthread_mutex_lock(&mutexS1);
			s1[indiceS1] = 0;
			pthread_mutex_unlock(&mutexS1);
		}
		printf("ho lo strumento s1[%d] \n", indiceS1);
		printf("INIZIO LAVORAZIONE 1 \n");
		sleep(1);
		indiceS2 = s2liberi();
		if(indiceS2 != -1){
			pthread_mutex_lock(&mutexS2);
			s2[indiceS2] = 0;
			pthread_mutex_unlock(&mutexS2);
		}
		else {
			printf("aspetto che si liberi un s2 \n");
			pthread_cond_wait(&s2Libero, &mutexS2);
			pthread_mutex_unlock(&mutexS2);
			indiceS2 = s2liberi();
			pthread_mutex_lock(&mutexS2);
			s2[indiceS2] = 0;
			pthread_mutex_unlock(&mutexS2);
		}
		printf("ho lo strumento s2[%d] \n", indiceS2);
		printf("CONTINUO LAVORAZIONE 1\n");
		sleep(2);
		printf("ora rilascio lo strumento s2[%d].. \n", indiceS2);
		pthread_mutex_lock(&mutexS2);
		s2[indiceS2] = 1; pthread_cond_signal(&s2Libero);
		pthread_mutex_unlock(&mutexS2);
		indiceS3 = s3liberi();
		if(indiceS3 != -1){
			pthread_mutex_lock(&mutexS3);
			s3[indiceS3] = 0; /* LO USO */
			pthread_mutex_unlock(&mutexS3);
		}
		else {
			printf("aspetto che si liberi un s3 \n");
			pthread_cond_wait(&s3Libero, &mutexS3);
			pthread_mutex_unlock(&mutexS3);
			indiceS3 = s3liberi();
			pthread_mutex_lock(&mutexS3);
			s3[indiceS3] = 0; /* LO USO */
			pthread_mutex_unlock(&mutexS3);
		}
		printf("ho lo strumento s3[%d] \n", indiceS3);
		printf("CONTINUO LAVORAZIONE 1\n");
		sleep(2);
		printf("LAVORAZIONE 1 FINITA, RILASCIO GLI STRUMENTI S1 e S3 \n");
		pthread_mutex_lock(&mutexS3); 
		s3[indiceS3] = 1; pthread_cond_signal(&s3Libero);
		pthread_mutex_unlock(&mutexS3);
		pthread_mutex_lock(&mutexS1);
		s1[indiceS1] = 1; pthread_cond_broadcast(&s1Libero);
		pthread_mutex_unlock(&mutexS1);
		printf("ASPETTO 3 SECONDI PRIMA DI REINIZIARE \n");
		sleep(3);
	}
}

void* lavorazione2(void* arg){
	int indiceS1; int indiceS2; int indiceS3;
	while(1){
		printf("inizio lavorazione 2.. \n");
		printf("prendo gli strumenti necessari \n");
		indiceS3 = s3liberi();
		if(indiceS3 != -1){
			pthread_mutex_lock(&mutexS3);
			s3[indiceS3] = 0; /* LO USO */
			pthread_mutex_unlock(&mutexS3);
		}
		else {
			printf("aspetto che si liberi un s3 \n");
			pthread_cond_wait(&s3Libero, &mutexS3);
			pthread_mutex_unlock(&mutexS3);
			indiceS3 = s3liberi();
			pthread_mutex_lock(&mutexS3);
			s3[indiceS3] = 0; /* LO USO */
			pthread_mutex_unlock(&mutexS3);
		}
		printf("ho lo strumento s3[%d] \n", indiceS3);
		printf("INIZIO LAVORAZIONE 2 \n");
		sleep(1);
		indiceS2 = s2liberi();
		if(indiceS2 != -1){
			pthread_mutex_lock(&mutexS2);
			s2[indiceS2] = 0;
			pthread_mutex_unlock(&mutexS2);
		}
		else {
			printf("aspetto che si liberi un s2 \n");
			pthread_cond_wait(&s2Libero, &mutexS2);
			pthread_mutex_unlock(&mutexS2);
			indiceS2 = s2liberi();
			pthread_mutex_lock(&mutexS2);
			s2[indiceS2] = 0;
			pthread_mutex_unlock(&mutexS2);
		}
		printf("ho lo strumento s2[%d] \n", indiceS2);
		printf("CONTINUO LAVORAZIONE 2\n");
		sleep(2);
		printf("ora rilascio lo strumento s2[%d].. \n", indiceS2);
		pthread_mutex_lock(&mutexS2);
		s2[indiceS2] = 1; pthread_cond_signal(&s2Libero);
		pthread_mutex_unlock(&mutexS2);
		indiceS1 = s1liberi();
		if(indiceS1 != -1){
			pthread_mutex_lock(&mutexS1);
			s1[indiceS1] = 0;
			pthread_mutex_unlock(&mutexS1);
		}
		else {
			printf("aspetto che si liberi un s1 \n ");
			pthread_mutex_lock(&mutexS1);
			pthread_cond_wait(&s1Libero, &mutexS1);
			pthread_mutex_unlock(&mutexS1);
			indiceS1 = s1liberi();
			pthread_mutex_lock(&mutexS1);
			s1[indiceS1] = 0;
			pthread_mutex_unlock(&mutexS1);
		}
		printf("ho lo strumento s1[%d] \n", indiceS1);
		printf("CONTINUO LAVORAZIONE 2 \n");
		sleep(1);
		printf("FINE LAVORAZIONE 2 \n");
		printf("ora rilascio strumento s1 e s3 \n");
		pthread_mutex_lock(&mutexS1);
		s1[indiceS1] = 1; pthread_cond_signal(&s1Libero);
		pthread_mutex_unlock(&mutexS1);
		pthread_mutex_lock(&mutexS3);
		s3[indiceS3] = 1; pthread_cond_signal(&s3Libero);
		pthread_mutex_unlock(&mutexS3);
		printf("ASPETTO 3 SECONDI PRIMA DI REINIZIARE \n");
		sleep(3);
	}
}

int main(void){
	int indice; pthread_t tid1[2]; pthread_t tid2[2];
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
		pthread_create(&tid1[indice], NULL, lavorazione1, NULL);
		sleep(1);
		pthread_create(&tid2[indice], NULL, lavorazione2, NULL);
	}
	pthread_exit(NULL);
}
