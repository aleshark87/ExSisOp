CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS=-lpthread -lrt
TARGET=staff_proc.exe 
OBJECTS= staff_proc.o


all:	${TARGET}

${TARGET}:	staff_proc.o 
			gcc ${CFLAGS} -o ${TARGET} ${OBJECTS}  ${LFLAGS}

${OBJECTS}: staff_proc.c	
		gcc -c staff_proc.c ${CFLAGS} -o ${OBJECTS}
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
 
 
 
 
 
 
 
 
