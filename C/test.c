#include <stdio.h>
#include "queue.h"
#include "global.h"
#include "dmio.h"
#include "mpi.h"

extern int proc_num = 12;
extern int client_num = 10;
extern int server_num = 1;
extern int master_num = 1;

int main(int argc, char **argv){
    MPI_Init(&argc, &argv); 
    int rank;
    MPI_Win nwin,nwin2,nwin3;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    dmio_init();
    set_status();
    double a[10][10];
    int i,j;
    for(i = 0;i<10;i++) {
        for(j=0;j<10;j++)
            a[i][j] = rank;
    }
    
    int ncid,varid;
    int ncid2,varid2;
    int ncid3,varid3;
    dmio_save("test_dmio.nc","mat",a, 10,10,&ncid, &varid, &nwin);
    dmio_save("test_file.nc","mat",a,10,10,&ncid2,&varid2,&nwin2);
    dmio_save("test_file2.nc","mat",a,10,10,&ncid3,&varid3,&nwin3);
    dmio_work();

    close_file(ncid);
    close_file(ncid2);
    close_file(ncid3);

    dmio_finalize();
    MPI_Finalize();

    return 0;
}
