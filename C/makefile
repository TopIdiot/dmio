dmio : dmio.o dmio_type.o queue.o
	mpicc dmio.o dmio_type.o queue.o -o dmio

dmio.o: dmio.h dmio.c  dmio_type.h dmio_type.c  queue.h queue.c
	mpicc -c dmio.c dmio_type.c queue.c

dmio_type.o: dmio_type.c dmio_type.h
	mpicc -c dmio_type.c

queue.o: queue.c queue.h 
	mpicc -c queue.c

clean:
	rm *.o dmio
