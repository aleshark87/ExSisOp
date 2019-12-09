typedef struct s{
	unsigned int index;
	pthread_t tid;
} S;

void * func ( void * arg );
