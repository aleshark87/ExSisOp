#define NUMS1 2
#define NUMS2 4
#define NUMS3 2
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int s1[NUMS1];
int s2[NUMS2];
int s3[NUMS3];

void* lavorazione1(void* arg){
	int indice;
	while(1){
		printf("inizio lavorazione 1... \n");
		printf("prendo gli strumenti necessari.\n");
		pthread_mutex_lock(&mutex);
		/*cerco uno strumento s1 libero*/
		for(indice = 0; indice < NUMS1; indice++){
			/*se lo strumento s1 è libero*/
			if(s1[indice] == 1){
				break;
			}
		}
		
	}
}

int main(void){
	int indice; pthread_t tid;
	for(indice = 0; indice < NUMS1; indice++){
		s1[indice] = 1;
	}
	for(indice = 0; indice < NUMS2; indice++){
		s2[indice] = 1;
	}
	for(indice = 0; indice < NUMS3; indice++){
		s3[indice] = 1;
	}
	pthread_create(&tid, NULL, lavorazione1, NULL);
	pthread_exit(NULL);
}
