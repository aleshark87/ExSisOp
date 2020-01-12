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

#include "DBGpthread.h"

#define NUMPERSONETAVOLO 4
#define NUMPERSONE 5
typedef struct sharedBuffer {
		pthread_mutex_t mutex;
		pthread_cond_t waitPizza ;
		pthread_cond_t waitTavolo ;
		int mangiando ;
		int personeAlTavolo ;
		
} SharedBuffer;
SharedBuffer *P;

void *pizza_lover(int arg){
	int indiceCl; int tempoGiro; indiceCl = arg;
	tempoGiro = 2 + indiceCl;
	while(1){
		DBGpthread_mutex_lock(&P->mutex);
		while((P->mangiando == 1) || (P->personeAlTavolo >= 4)){
			DBGpthread_cond_wait(&P->waitPizza, &P->mutex);
		}
		P->personeAlTavolo++; 
		if(P->personeAlTavolo < 4){
			printf("[%d]Aspetto di essere tutti... \n", indiceCl);
			DBGpthread_cond_wait(&P->waitTavolo, &P->mutex);
		}
		else{
			printf("[%d]SIAMO TUTTI, POSSIAMO MANGIARE !!. \n", indiceCl);
			DBGpthread_cond_broadcast(&P->waitTavolo);
		}
		DBGpthread_mutex_unlock(&P->mutex);
		sleep(6);
		DBGpthread_mutex_lock(&P->mutex);
		P->personeAlTavolo--; P->mangiando = 0;
		if(P->personeAlTavolo == 0){
			printf("TAVOLO LIBERO ! \n"); 
			DBGpthread_cond_broadcast(&P->waitPizza);
		}
		printf("[%d]TORNO IN %d . \n", indiceCl, tempoGiro); 
		DBGpthread_mutex_unlock(&P->mutex);
		sleep(tempoGiro);
	}
}

int main(void){
	int shmfd, rc, i;
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

	rc = pthread_cond_init(&P->waitPizza, &cvattr );
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_cond_init(&P->waitTavolo, &cvattr );
	if( rc ) PrintERROR_andExit(rc,"pthread_cond_init failed");
	rc = pthread_mutex_init(&P->mutex, &mattr ); 
	if( rc ) PrintERROR_andExit(rc,"pthread_mutex_init failed");

	P->mangiando = 0;

	/*creo i thread alfa*/
	for(i=0;i<NUMPERSONE;i++) {
		pid=fork();
		if(pid<0) 
			PrintERROR_andExit(errno,"fork failed");
		else if(pid==0) {
			pizza_lover(i);
			exit(0);
		}
	}
	return 0;
}
