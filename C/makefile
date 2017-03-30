test_f: test_fortran.f90 dmio_fortran.f90 dmio_f.o interface_fortran.o libdmio_c_interface.so
	mpiifort dmio_f.o interface_fortran.o  test_fortran.f90 -o test_f -L. -ldmio_c_interface 

dmio_f.o:dmio_fortran.f90 interface_fortran.o 
	mpiifort -c  interface_fortran.o  dmio_fortran.f90  -o dmio_f.o 

interface_fortran.o: interface_fortran.c interface_fortran.h interface_c.h 
	mpiicc -c interface_fortran.c  -o interface_fortran.o 


libdmio_c_interface.so:interface_c.cpp  dmio.cpp global.cpp global.h dmio.h interface_c.h
	mpiicc  interface_c.cpp dmio.cpp global.cpp -fpic -shared -o libdmio_c_interface.so -I${INTEL_ESM_SOFT}/include -I/usr/include -L${INTEL_ESM_SOFT}/lib -lpnetcdf 

test_c : test.c interface_c.h interface_c.cpp libdmio_c_interface.so
	mpiicc test.c interface_c.cpp  -o test_c  -I${INTEL_ESM_SOFT}/include -I/usr/include -L${INTEL_ESM_SOFT}/lib -lpnetcdf -L. -ldmio_c_interface


clean:
	-rm *.o test_c test_f test_dmio.nc core* *.nc *.so

run: 
	mpirun -n 12 ./test

view:
	ncmpidump test_dmio.nc

cess:
	qsub run.sh

sunway_c:
	-make test_c
	bsub -I  -q q_x86_share  -n 222 ./test_c

sunway_f:
	-make test_f
	bsub -I  -q q_x86_share  -n 222 ./test_f

cleanlog:
	rm dmio_test.o*
