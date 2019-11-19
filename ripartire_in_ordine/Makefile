CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_POSIX_C_SOURCE=210112L
LFLAGS=-lpthread
TARGET=rip_in_ord.exe
OBJECTS=rip_in_ord.o


all:	${TARGET}

${TARGET}:	${OBJECTS} 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS} ${LFLAGS}

rip_in_ord.o:	ripartire_in_ordine.c	
				gcc -c ripartire_in_ordine.c ${CFLAGS} -o rip_in_ord.o
		
.PHONY:	clean

clean:
		-rm ${TARGET} ${OBJECTS} 
		

 

 
