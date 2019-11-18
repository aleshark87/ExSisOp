CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=es_banche2.exe 
OBJECTS= es_banche2.o


all:	${TARGET}

es_banche2.exe:	es_banche2.o 
			gcc ${CFLAGS} -o ${TARGET} es_banche2.o  ${LFLAGS}

${OBJECTS}:	es_banche2.c	
		gcc -c es_banche2.c ${CFLAGS} -o es_banche2.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
