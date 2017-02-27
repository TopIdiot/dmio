test : test.o dmio.o global.o queue.o
	mpiicc test.o dmio.o global.o queue.o -o test  -I/home/share/cesm/software/esm-soft/include -I/usr/include -L/home/share/cesm/software/esm-soft/lib -lpnetcdf


test.o:test.c  dmio.h  global.h queue.h
	mpiicc -c test.c -I/home/share/cesm/software/esm-soft/include -I/usr/include -L/home/share/cesm/software/esm-soft/lib -lpnetcdf

dmio.o: dmio.c dmio.h  global.h  queue.h 
	mpiicc -c dmio.c  -I/home/share/cesm/software/esm-soft/include -I/usr/include -L/home/share/cesm/software/esm-soft/lib -lpnetcdf

global.o: global.c global.h
	mpiicc -c global.c -I/home/share/cesm/software/esm-soft/include -I/usr/include -L/home/share/cesm/software/esm-soft/lib -lpnetcdf

queue.o: queue.c queue.h 
	mpiicc -c queue.c -I/home/share/cesm/software/esm-soft/include -I/usr/include -L/home/share/cesm/software/esm-soft/lib -lpnetcdf

clean:
	rm *.o test test_dmio.nc core* *.nc

run: 
	mpirun -n 12 ./test

view:
	ncmpidump test_dmio.nc
