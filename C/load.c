#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "pnetcdf.h"

int main(int argc, char **argv){

    int i,j,rank,nprocs,ret;
    int ncfile, ndims, nvars, ngatts, unlimited;
    MPI_Offset *
    ncmpi_open(MPI_COMM_WORLD, filename, NC_NOWRITE, MPI_INFO_NULL, &ncfile);

    ncmpi_inq(ncfile, &ndims, &nvars, &ngatts, &unlimited);


}
