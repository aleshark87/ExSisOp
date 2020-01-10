CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=pizzeria.exe 
OBJECTS= pizzeria.o


all:	${TARGET}

${TARGET}:	pizzeria.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: pizzeria.c	
		gcc -c pizzeria.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
 
 
 
