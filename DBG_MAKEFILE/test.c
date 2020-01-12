#include "printerror.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include "DBGpthread.h"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* dumbFunction(void* arg){
	DBGpthread_mutex_lock(&mutex);
	printf("MUTEX.\n");
	sleep(1);
	DBGpthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}
int main(void){
	pthread_t tid; int rc; char msg[256];
	rc = pthread_create(&tid, NULL, dumbFunction, NULL);
	if(rc){
		PrintERROR_andExit(rc, msg);
	}
	pthread_exit(NULL);
	return 0;
}
