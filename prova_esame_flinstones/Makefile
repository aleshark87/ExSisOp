CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread
TARGET=flinstone.exe 
OBJECTS= flinstone.o


all:	${TARGET}

${TARGET}:	flinstone.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: flinstones.c	
		gcc -c flinstones.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
