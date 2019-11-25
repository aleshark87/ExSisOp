CFLAGS=-ansi -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L

LIBRARIES=-lpthread

all: 	SyncPoint_a_coppie_CORRETTO.exe

SyncPoint_a_coppie_CORRETTO.exe: SyncPoint_a_coppie_CORRETTO.o DBGpthread.o
	gcc ${CFLAGS} -o SyncPoint_a_coppie_CORRETTO.exe SyncPoint_a_coppie_CORRETTO.o DBGpthread.o ${LIBRARIES}

SyncPoint_a_coppie_CORRETTO.o: SyncPoint_a_coppie_CORRETTO.c DBGpthread.h printerror.h
	gcc ${CFLAGS} -c SyncPoint_a_coppie_CORRETTO.c

DBGpthread.o: DBGpthread.c printerror.h
	gcc ${CFLAGS} -c DBGpthread.c

.PHONY:	clean

clean:
	-rm -f SyncPoint_a_coppie_CORRETTO.o DBGpthread.o SyncPoint_a_coppie_CORRETTO.exe


