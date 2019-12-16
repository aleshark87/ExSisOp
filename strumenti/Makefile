CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=strumenti.exe 
OBJECTS= strumenti.o


all:	${TARGET}

${TARGET}:	strumenti.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: strumenti.c	
		gcc -c strumenti.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
