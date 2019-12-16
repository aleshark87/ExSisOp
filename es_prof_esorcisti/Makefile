CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=scalino_esorcisti.exe 
OBJECTS= scalino_esorcisti.o


all:	${TARGET}

${TARGET}:	scalino_esorcisti.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: scalino_esorcisti.c	
		gcc -c scalino_esorcisti.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
