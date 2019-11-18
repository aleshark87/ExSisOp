CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=fachiri.exe 
OBJECTS= fachiri.o


all:	${TARGET}

${TARGET}:	${OBJECTS}
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}:	fachiri.c	
		gcc -c fachiri.c ${CFLAGS} -o fachiri.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
