#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NUMPOSTI 2
#define NUMCAVERNICOLI_A 2
#define NUMCAVERNICOLI_B 1
#define tempoTrasporto 2 
#define tempoGiretto 4 
#define numCavernicoli 3 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condDinoArrivato = PTHREAD_COND_INITIALIZER;
pthread_cond_t condPasseggeriScesi = PTHREAD_COND_INITIALIZER;
pthread_cond_t condPasseggeriSaliti = PTHREAD_COND_INITIALIZER;
pthread_cond_t condAttendiDinoLatoA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condAttendiDinoLatoB = PTHREAD_COND_INITIALIZER;

int NumPostiLiberi=NUMPOSTI;
int latoASalire=0,
    latoBSalire=0;


void* cavernicolo(void *arg){
	char lato;
	int indice;
	indice = * (int*) arg;
	if( indice<NUMCAVERNICOLI_A )	lato='A';
	else 	lato='B';
	while(1){
		pthread_mutex_lock(&mutex);
		if(lato == 'A'){
			while(latoASalire == 0 || NumPostiLiberi == 0){
				pthread_cond_wait(&condAttendiDinoLatoA,&mutex);
			}
		}
		else{
			while(latoBSalire == 0 || NumPostiLiberi == 0){
				pthread_cond_wait(&condAttendiDinoLatoB,&mutex);
			}
		}
		printf("SALGO , cav numero %d\n",indice);
		fflush(stdout);
		
		NumPostiLiberi--;
		if(NumPostiLiberi == 0){
			printf("DINOSAURO , PARTI !\n");
			pthread_cond_signal(&condPasseggeriSaliti);
		}
		pthread_cond_wait(&condDinoArrivato,&mutex);
		printf("SCENDO, cav numero %d\n",indice);
		if(NumPostiLiberi == NUMPOSTI){
			pthread_cond_signal(&condPasseggeriScesi);
		}
		/*cambio il lato*/
		if(lato == 'A'){
			lato = 'B';
		}
		else{
			lato = 'A';
		}
		pthread_mutex_unlock(&mutex);
		sleep(tempoGiretto);
	}
	
}

void* dinosauro(void *arg){
	char lato='A';
	pthread_mutex_lock(&mutex);
	while(1){
		/* Il dinosauro vuole ripartire: 
		 * avvisa di salire i cavernicoli
		 * dal lato giusto */
		printf("voglio ripartire.. lato %c \n",lato);
		fflush(stdout);
		if(lato == 'A'){
			latoASalire = 1;
			pthread_cond_broadcast(&condAttendiDinoLatoA);
		}
		else{
			latoBSalire = 1;
			pthread_cond_broadcast(&condAttendiDinoLatoB);
		}
		NumPostiLiberi = NUMPOSTI;
		printf("SALITE \n");
		fflush(stdout);
		pthread_cond_wait(&condPasseggeriSaliti,&mutex);
		latoASalire = 0;
		latoBSalire = 0;
		printf(" PARTO DA LATO %c\n",lato);
		fflush(stdout);
		pthread_mutex_unlock(&mutex);
		sleep(tempoTrasporto);
		if(lato == 'A') lato = 'B';
		else lato = 'A';
		pthread_mutex_lock(&mutex);
		printf(" ARRIVATO SU LATO %c\n",lato);
		fflush(stdout);
		printf(" SONO ARRIVATO \n");
		pthread_cond_wait(&condDinoArrivato,&mutex);
		printf(" SCENDETE \n");
		fflush(stdout);
		pthread_cond_wait(&condPasseggeriScesi,&mutex);
	}
	
}

int main(void){
	pthread_t tid;
	int i;
	for(i = 0; i < numCavernicoli; i++){
		int *p;
		p = (int*)malloc(sizeof(int));
		*p = i;
		pthread_create(&tid, NULL, cavernicolo, (void*)p);
	}
	pthread_create(&tid, NULL, dinosauro, NULL);
	pthread_exit(NULL);
	return 0;
}
	
	
	
	

