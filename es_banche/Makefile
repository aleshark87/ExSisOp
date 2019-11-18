CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=es_banche.exe 
OBJECTS= es_banche.o


all:	${TARGET}

es_banche.exe:	es_banche.o 
			gcc ${CFLAGS} -o ${TARGET} es_banche.o  ${LFLAGS}

${OBJECTS}:	es_banche.c	
		gcc -c es_banche.c ${CFLAGS} -o es_banche.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
