CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_POSIX_C_SOURCE=210112L
LFLAGS=-lpthread
TARGET=sinc_circ.exe
OBJECTS=sinc_circ.o


all:	${TARGET}

${TARGET}:	${OBJECTS} 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} ${LFLAGS}

sinc_circ.o:	sinc_circ.c	
				gcc -c sinc_circ.c ${CFLAGS} -o sinc_circ.o
		
.PHONY:	clean

clean:
		-rm ${TARGET} ${OBJECTS} 
		

 

 
 
