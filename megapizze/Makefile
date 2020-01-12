CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread -lrt
TARGET=megapizze.exe 
OBJECTS=megapizze.o


all:	${TARGET}

${TARGET}:	megapizze.o  DBGpthread.o
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} DBGpthread.o  ${LFLAGS}

${OBJECTS}: megapizze.c	DBGpthread.h printerror.h
		gcc -c megapizze.c ${CFLAGS} -o ${OBJECTS}

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
