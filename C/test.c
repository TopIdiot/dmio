#include <stdio.h>
#include "queue.h"
#include "dmio_type.c"
#include "dmio.h"
#include "mpi.h"

proc_num = 12
client_num = 10;
server_num = 1;

int main(int argc, char **argv){
    MPI_Init(&argc, &argv); 
    dmio_init();
    int a[10][10]={};
    int ncid,varid;
    
    dmio_save("test_dmio","mat",a, 10,10, &ncid, &varid);
    server_work();
    close_file(ncid);
    dmio_finalize();
    MPI_Finalize();
}


