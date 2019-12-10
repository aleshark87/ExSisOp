CFLAGS= -ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
LFLAGS= -lpthread
TARGET=A+B_Cons_Prod.exe 
OBJECTS= A+B_Cons_Prod.o

all:	${TARGET}

A+B_Cons_Prod.exe:	A+B_Cons_Prod.o 
			gcc ${CFLAGS} -o ${TARGET} A+B_Cons_Prod.o  ${LFLAGS}

${OBJECTS}:	A+B_Cons_Prod.c	
		gcc -c A+B_Cons_Prod.c ${CFLAGS} -o A+B_Cons_Prod.o
		
.PHONY: clean

clean:
		-rm ${TARGET} ${OBJECTS}
 
