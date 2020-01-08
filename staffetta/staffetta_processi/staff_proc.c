#define NUMALFA 5
#define NUMBETA 4
#define ALFA 0
#define BETA 1
#define myMSGLEN 128
#include "printerror.h"
#include <unistd.h>   /* exit() etc */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>     /* per strerror_r  and  memset */
#include <sys/mman.h>  /* shm_* stuff, and mmap() */
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h> /* timeval{} for select() */
#include <time.h> /* timespec{} for pselect() */
#include <limits.h> /* for OPEN_MAX */
#include <errno.h>
#include <sys/wait.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <pthread.h> 

typedef struct sharedBuffer{
		pthread_mutex_t mutex;
		pthread_cond_t waitA;
		pthread_cond_t waitB;
		int numGiriA[NUMALFA];
		int numGiriB[NUMBETA];
		int turno;
		int pistaOccupata;
} SharedBuffer;
SharedBuffer *P;

/*ritorna 1 se l'atleta indice non può prendere il testimone */
/*ritorna 0 se l'atleta indice può prendere il testimone */
int shallNotPass(int indice, int squadra){
	int i; int check = 0;
	if(squadra == ALFA){
		for(i = 0; i < indice; i++){
			if(P->numGiriA[indice] > (P->numGiriA[i] + 1)){
				 check += 1;
			}
		}
		for(i = (indice + 1); i < NUMALFA; i++){
			if(P->numGiriA[indice] > (P->numGiriA[i] + 1)){
				check += 1;
			}
		}
	}
	else {
		for(i = 0; i < indice; i++){
			if(P->numGiriB[indice] > (P->numGiriB[i] + 1)){
				 check += 1;
			}
		}
		for(i = (indice + 1); i < NUMBETA; i++){
			if(P->numGiriB[indice] > (P->numGiriB[i] + 1)){
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

void* atletaALFA(int indiceAtleta){
	/*int indiceAtleta; indiceAtleta = *(int*)arg;*/
	while(1){
		pthread_mutex_lock(&P->mutex);
		while((shallNotPass(indiceAtleta, ALFA)) || (P->turno == BETA) || (P->pistaOccupata == 1)){
			pthread_cond_wait(&P->waitA, &P->mutex);
		}
		P->pistaOccupata = 1;
		pthread_mutex_unlock(&P->mutex);
		printf("ATLETA A %d inizia il GIRO, numero Giri = %d \n", indiceAtleta, P->numGiriA[indiceAtleta]);
		sleep(1);
		printf("PASSO TESTIMONE A SQUADRA BETA \n");
		pthread_mutex_lock(&P->mutex); P->numGiriA[indiceAtleta]++; P->turno = BETA; P->pistaOccupata = 0;
		pthread_cond_broadcast(&P->waitB); pthread_mutex_unlock(&P->mutex);
	}
}
void* atletaBETA(int indiceAtleta){
	/*int indiceAtleta; indiceAtleta = *(int*) arg;*/
	while(1){
		pthread_mutex_lock(&P->mutex);
		while((shallNotPass(indiceAtleta, BETA)) || (P->turno == ALFA) || (P->pistaOccupata == 1)){
			pthread_cond_wait(&P->waitB, &P->mutex);
		}
		P->pistaOccupata = 1;
		pthread_mutex_unlock(&P->mutex);
		printf("ATLETA B %d inizia il GIRO , numero Giri = %d \n", indiceAtleta, P->numGiriB[indiceAtleta]);
		sleep(1);
		printf("PASSO TESTIMONE A SQUADRA ALFA \n");
		pthread_mutex_lock(&P->mutex); P->numGiriB[indiceAtleta]++; P->turno = ALFA; P->pistaOccupata = 0;
		pthread_cond_broadcast(&P->waitA); pthread_mutex_unlock(&P->mutex);
	}
}
int main(void){
	int shmfd, rc, indice;
	pid_t pid;
	int shared_seg_size = sizeof(SharedBuffer);
	pthread_mutexattr_t mattr;
	pthread_condattr_t cvattr;
	
	shmfd = shm_open( "/pedala", O_CREAT /*| O_EXCL*/ | O_RDWR, S_IRWXU );
	if (shmfd < 0) {
		perror("In shm_open()");
		exit(1);
	}
	/* adjusting mapped file size (make room for the whole segment to map) */
	rc = ftruncate(shmfd, shared_seg_size);
	if (rc != 0) {
		perror("ftruncate() failed");
		exit(1);
	}

	P = (SharedBuffer*)mmap(NULL, shared_seg_size,
		PROT_READ|PROT_WRITE, MAP_SHARED, shmfd, 0);
	if( P==MAP_FAILED )
	 		PrintERROR_andExit(errno,"mmap  failed");

	rc=pthread_mutexattr_init(&mattr);
	if( rc ) PrintERROR_andExit(rc,"pthread_mutexattr_init  failed");
	rc=pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
	if( rc ) PrintERROR_andExit(rc,"pthread_mutexattr_setpshared  failed");
	rc=pthread_condattr_init(&cvattr);
	if( rc ) PrintERROR_andExit(rc,"pthread_condattr_init  failed");
	rc=pthread_condattr_setpshared(&cvattr,PTHREAD_PROCESS_SHARED);
	if( rc ) PrintERROR_andExit(rc,"pthread_condattr_setpshared  failed");

	rc = pthread_cond_init(&P->waitA, &cvattr );
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&P->waitB, &cvattr );
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&P->mutex, &mattr ); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");
	
	P->turno = ALFA; P->pistaOccupata = 0;
	for(indice = 0; indice < NUMALFA; indice++){
		P->numGiriA[indice] = 0;
	}
	for(indice = 0; indice < NUMBETA; indice++){
		P->numGiriB[indice] = 0;
	}
	
	/*creo i thread alfa*/
	for(indice=0;indice<NUMALFA;indice++) {
		pid=fork();
		if(pid<0) 
			PrintERROR_andExit(errno,"fork failed");
		else if(pid==0) {
			atletaALFA(indice);
			exit(0);
		}
	}


	/*creo i thread beta*/
	for(indice=0;indice<NUMBETA;indice++) {
		pid=fork();
		if(pid<0) 
			PrintERROR_andExit(errno,"fork failed");
		else if(pid==0) {
			atletaBETA(indice);
			exit(0);
		}
	}

	return 0;
}
