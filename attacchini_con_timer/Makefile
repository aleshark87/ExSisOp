CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=attacchini.exe 
OBJECTS=attacchini.o


all:	${TARGET}

${TARGET}:	attacchini.o  DBGpthread.o
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} DBGpthread.o  ${LFLAGS}

${OBJECTS}: attacchini.c	DBGpthread.h printerror.h
		gcc -c attacchini.c ${CFLAGS} -o ${OBJECTS}

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS} DBGpthread.o 
 
 
 
 
 
 
