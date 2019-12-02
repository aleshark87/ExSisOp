CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_POSIX_C_SOURCE=210112L
LFLAGS=-lpthread
TARGET=merda.exe
OBJECTS=merda.o


all:	${TARGET}

${TARGET}:	${OBJECTS} 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} ${LFLAGS}

merda.o:	merda.c	
				gcc -c merda.c ${CFLAGS} -o merda.o
		
.PHONY:	clean

clean:
		-rm ${TARGET} ${OBJECTS} 
		

 

 
 
 
 
