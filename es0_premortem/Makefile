CFLAGS=-ansi -Wpedantic -Wall -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L -D_DEFAULT_SOURCE
LFLAGS=-lpthread
TARGET=es0_premortem.exe
OBJECTS= funzioni.o main.o

all:	${TARGET}

${TARGET}:	main.o funzioni.o
			gcc ${OBJECTS} ${CFLAGS} -o ${TARGET} ${LFLAGS}

main.o:	main.c
		gcc -c main.c ${CFLAGS} -o main.o

funzioni.o:	funzioni.c funzioni.h
			gcc -c funzioni.c ${CFLAGS} -o funzioni.o

.PHONY:	clean

clean:
		-rm ${TARGET} ${OBJECTS}
