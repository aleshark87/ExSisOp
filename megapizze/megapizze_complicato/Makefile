CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread -lrt
TARGET=mega_compl.exe 
OBJECTS=mega_compl.o


all:	${TARGET}

${TARGET}:	mega_compl.o  DBGpthread.o
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} DBGpthread.o  ${LFLAGS}

${OBJECTS}: mega_compl.c	DBGpthread.h printerror.h
		gcc -c mega_compl.c ${CFLAGS} -o ${OBJECTS}

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
