CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=fornaio.exe 
OBJECTS= fornaio.o


all:	${TARGET}

${TARGET}:	${OBJECTS}
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}:	fornaio_doppio.c	
		gcc -c fornaio_doppio.c ${CFLAGS} -o fornaio.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS} 
