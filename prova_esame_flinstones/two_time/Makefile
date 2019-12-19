 
CFLAGS= -ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS= -lpthread
TARGET=flinstone.exe 
OBJECTS= flinstone.o

all:	${TARGET}

flinstone.exe:	flinstone.o 
			gcc ${CFLAGS} -o ${TARGET} flinstone.o  ${LFLAGS}

${OBJECTS}:	flinstone.c	
		gcc -c flinstone.c ${CFLAGS} -o flinstone.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
