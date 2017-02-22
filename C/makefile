all: dmio

dmio : dmio.o dmio_type.o queue.o
	mpicc dmio.o dmio_type.o queue.o -o dmio

queue.o: queue.c queue.h dmio_type.h 
	mpicc -c queue.c -o queue.o

dmio_type.o: dmio_type.c dmio_type.h
	mpicc -c dmio_type.c -o dmio_type.o

dmio.o:  dmio.c  dmio_type.h dmio_type.c  queue.h queue.c
	mpicc -c dmio.c -o dmio.o


