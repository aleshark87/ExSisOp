CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=test.exe 
OBJECTS=test.o


all:	${TARGET}

${TARGET}:	test.o  DBGpthread.o
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} DBGpthread.o  ${LFLAGS}

${OBJECTS}: test.c	DBGpthread.h printerror.h
		gcc -c test.c ${CFLAGS} -o ${OBJECTS}

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
