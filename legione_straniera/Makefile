CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread -lrt
TARGET=legione.exe 
OBJECTS= legione.o


all:	${TARGET}

${TARGET}:	legione.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: legione.c	
		gcc -c legione.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
 
 
 
 
