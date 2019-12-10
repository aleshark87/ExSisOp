CFLAGS= -ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS= -lpthread
TARGET=arzigogolo.exe 
OBJECTS= arzigogolo.o

all:	${TARGET}

arzigogolo.exe:	arzigogolo.o 
			gcc ${CFLAGS} -o ${TARGET} arzigogolo.o  ${LFLAGS}

${OBJECTS}:	arzigogolo.c	
		gcc -c arzigogolo.c ${CFLAGS} -o arzigogolo.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
