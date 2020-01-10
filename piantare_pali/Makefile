CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread -lrt
TARGET=piantare.exe 
OBJECTS= piantare.o


all:	${TARGET}

${TARGET}:	piantare.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: piantare.c	
		gcc -c piantare.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
 
 
 
