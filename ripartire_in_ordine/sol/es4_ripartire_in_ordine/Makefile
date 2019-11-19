# Makefile per mutex e CondVar* 
# a causa della presenza di strerror_r in printerror.h
# occorre compilare col flag -D_POSIX_C_SOURCE=200112L

CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread

all: 	es4_ripartire_in_ordine.exe

es4_ripartire_in_ordine.exe: es4_ripartire_in_ordine.o
	gcc ${CFLAGS} -o es4_ripartire_in_ordine.exe es4_ripartire_in_ordine.o ${LIBRARIES}

es4_ripartire_in_ordine.o: es4_ripartire_in_ordine.c printerror.h
	gcc ${CFLAGS} -c es4_ripartire_in_ordine.c

.PHONY:	clean

clean:
	rm -f es4_ripartire_in_ordine.o es4_ripartire_in_ordine.exe

