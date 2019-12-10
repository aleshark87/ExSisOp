# Makefile per mutex e CondVar* 
# a causa della presenza di strerror_r in printerror.h
# occorre compilare col flag -D_POSIX_C_SOURCE=200112L

CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread

all: 	es10_circolareA3_vet3cond.exe

es10_circolareA3_vet3cond.exe: es10_circolareA3_vet3cond.o DBGpthread.o
	gcc ${CFLAGS} -o es10_circolareA3_vet3cond.exe es10_circolareA3_vet3cond.o DBGpthread.o ${LIBRARIES}

es10_circolareA3_vet3cond.o: es10_circolareA3_vet3cond.c printerror.h
	gcc ${CFLAGS} -c es10_circolareA3_vet3cond.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	rm -f es10_circolareA3_vet3cond.o DBGpthread.o es10_circolareA3_vet3cond.exe

