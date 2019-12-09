#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "funzioni.h"


int main() {
	/*variable declaration*/
	int rc;
	S *p;
	pthread_t tid;
	
	printf("Main\n");
	fflush(stdout);
	usleep(1000);
	p = malloc(sizeof(S));
	p->index = 0;
	p->tid = pthread_self();
	printf("creating new thread..");
	rc = pthread_create( &tid, NULL, func, (void*) p);
	/*check on creation*/
	if(rc){
		char msg[256];
		strerror_r(rc, msg, 256);
		printf("pthread_create failed, %s\n",msg);
	}
	printf("end main\n");
	fflush(stdout);
	pthread_exit(NULL);
}
	
	
