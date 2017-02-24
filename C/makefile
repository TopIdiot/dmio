test : dmio.o dmio_type.o queue.o
	mpicc dmio.o dmio_type.o queue.o -o dmio -I /home/isco/PnetCDF/include

test.o:test.c dmio.c dmio.h dmio_type.c dmio_type.h queue.c queue.h
	mpicc -c test.c -I /home/isco/PnetCDF/include -L /home/isco/PnetCDF/lib -lpnetcdf

dmio.o: dmio.h dmio.c  dmio_type.h dmio_type.c  queue.h queue.c
	mpicc -c dmio.c  -I /home/isco/PnetCDF/include -L /home/isco/PnetCDF/lib -lpnetcdf

dmio_type.o: dmio_type.c dmio_type.h
	mpicc -c dmio_type.c

queue.o: queue.c queue.h 
	mpicc -c queue.c

clean:
	rm *.o dmio
