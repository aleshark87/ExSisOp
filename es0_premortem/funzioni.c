#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "funzioni.h"


void * func ( void * arg ) {
	/*variable declaration*/
	void* takeInfo;
	int rc;
	pthread_t tid;
	S *p1;
	S *p = (S*) arg ; /*arg was an S type :) , to be a pointer of S (S*)*/
	
	printf("index = %d \n", p->index);
	fflush(stdout);
	usleep(1000);
	
	p1 = malloc(sizeof(S));
	if(p1 == NULL) {
		printf("malloc failed\n");
		exit(1);
	}
	p1->index = p->index + 1; /*take the old index , and makes plus 1*/
	p1->tid = pthread_self(); /*pthread_self gives the id of actual thread*/
		
	rc = pthread_create(&tid, NULL, func, (void*)p1); 
	/*&tid because pthread_create wants a pointer */
	/*control on creation of a new thread, if cant create pthread, exit*/
	if(rc) {
		char msg[256];
		strerror_r(rc, msg, sizeof(msg));
		puts(msg);
		exit(1);
	}
	
	rc = pthread_join(p->tid, &takeInfo);
	/*secund argument of pthread_join returns a double pointer of the
	 * value returned by pthread_exit. We need his address because
	 * retvalue is a double pointer*/
	if(rc != 0) {
		char msg[256];
		strerror_r(rc, msg, sizeof(msg));
		puts(msg);
		exit(1);
	}
	pthread_exit(NULL);
}
	
	
	
	
