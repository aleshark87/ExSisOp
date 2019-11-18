CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_POSIX_C_SOURCE=210112L
LFLAGS=-lpthread
TARGET=fachiri3.exe
OBJECTS=fachiri3.o


all:	${TARGET}

${TARGET}:	${OBJECTS} 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} ${LFLAGS}

fachiri3.o:	fachiri3.c	
				gcc -c fachiri3.c ${CFLAGS} -o fachiri3.o
		
.PHONY:	clean

clean:
		-rm ${TARGET} ${OBJECTS} 
		

 

