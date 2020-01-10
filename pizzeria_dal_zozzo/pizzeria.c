#define N 10
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>  

int pizzeSulBancone = N; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitPizzaiolo = PTHREAD_COND_INITIALIZER;
pthread_cond_t waitPizza = PTHREAD_COND_INITIALIZER;

int banconePieno(){
	return pizzeSulBancone == N;
}
int banconeVuoto(){
	return pizzeSulBancone == 0;
}

void* pizzaiolo(void* arg){
	int pizzePreparate = 0;
	while(1){
		pizzePreparate++;
		pthread_mutex_lock(&mutex);
		while(banconePieno()){
			pthread_cond_wait(&waitPizzaiolo, &mutex);
		}
		printf("PREPARO 1 PIZZA. PIZZE SUL BANCONE = %d  \n", pizzeSulBancone);
		pthread_mutex_unlock(&mutex);
		sleep(1);
		pthread_mutex_lock(&mutex); pizzeSulBancone++;
		pthread_cond_broadcast(&waitPizza);
		pthread_mutex_unlock(&mutex);
	}
}

void* cliente(void* arg){
	while(1){
		pthread_mutex_lock(&mutex);
		while(banconeVuoto()){
			pthread_cond_signal(&waitPizzaiolo);
			pthread_cond_wait(&waitPizza, &mutex); 
		}
		pthread_mutex_unlock(&mutex);
		puts("MI MANGIO UNA PIZZA. ");
		sleep(2);
		pthread_mutex_lock(&mutex); pizzeSulBancone--; pthread_mutex_unlock(&mutex);
	}
}
int main(void){
	pthread_t tid;
	pthread_create(&tid, NULL, pizzaiolo, NULL);
	pthread_create(&tid, NULL, cliente, NULL);
	pthread_exit(NULL);
}
