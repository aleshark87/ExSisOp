CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=guado_zambesi.exe 
OBJECTS=guado_zambesi.o


all:	${TARGET}

${TARGET}:	guado_zambesi.o  DBGpthread.o
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} DBGpthread.o  ${LFLAGS}

${OBJECTS}: guado_zambesi.c	DBGpthread.h printerror.h
		gcc -c guado_zambesi.c ${CFLAGS} -o ${OBJECTS}

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS} DBGpthread.o 
 
 
 
 
 
 
