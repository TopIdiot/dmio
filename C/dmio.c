#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "mpi.h"
#include "dmio.h"
#include <stdlib.h>
#include "global.h"
#include "queue.h"
#include "pnetcdf.h"
//queue_status 0 empty 1 not empty
//io_status 0 io end  1 io not end 

#define NC_ERR {if(ierr!=NC_NOERR){printf("Error at line=%d: %s\n", __LINE__, ncmpi_strerror(ierr));}}

void set_status()
{
    queue_status = is_free;
    io_status = is_busy;
    master_status = is_free;
    server_status = is_free;
    tot_request = 0;
    now_request = 0;
    origin_tag = 0;
}

void create_msg_type()
{
    int blocklength[] = {1,1,1,1,1,1};
    MPI_Aint offsets[] = {0,sizeof(MPI_INT),2*sizeof(MPI_INT),3*sizeof(MPI_INT), 4*sizeof(MPI_INT), 5*sizeof(MPI_INT)};
    MPI_Datatype oldtypes[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Type_struct(6,blocklength,offsets,oldtypes,&message);
    MPI_Type_commit(&message);
}

void dmio_init()
{
    int rank,size;
    int *ranks1;
    int *ranks2;
    int *ranks3;
    int i;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    proc_type = get_proc_type(rank);
    server_num = size - master_num - client_num;
    ranks1 = malloc(sizeof(int)*size);
    ranks2 = malloc(sizeof(int)*size);
    ranks3 = malloc(sizeof(int)*size);

    for(i = 0;i<server_num;i++) ranks1[i] = i + client_num + master_num;
   
    ranks2[0] = 0;

    for(i = 1;i<=server_num;i++) ranks2[i] = i + client_num;
   
    for(i=0;i<size-1;i++) ranks3[i] = i + master_num;

    MPI_Comm_group(MPI_COMM_WORLD,&group);
    MPI_Group_incl(group, server_num, ranks1, &only_server_group);
    MPI_Comm_create(MPI_COMM_WORLD, only_server_group, &only_server_comm);

    MPI_Group_incl(group, server_num+1, ranks2, &server_and_master_group);
    MPI_Comm_create(MPI_COMM_WORLD, server_and_master_group, &server_and_master_comm); 

    MPI_Group_incl(group, client_num+server_num, ranks3, &server_and_client_group); 
    MPI_Comm_create(MPI_COMM_WORLD,server_and_client_group,&server_and_client_comm);

    free(ranks1);
    free(ranks2);
    free(ranks3);
    
    init_queue(&queue);
    init_queue(&msg_buffer);
    create_msg_type();
}

void dmio_save(char* filename, char* varname, double *A, int nx, int ny, int *ncid, int *varid, MPI_Win *nwin ){

    tot_request += client_num;

    int ierr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   
    ierr = MPI_Win_create(A,sizeof(double)*nx*ny,sizeof(double),MPI_INFO_NULL,MPI_COMM_WORLD,nwin);

    if(proc_type == proc_server){
        int cmode,dimid[2];
        cmode = NC_CLOBBER | NC_64BIT_DATA;
        ierr = ncmpi_create(only_server_comm, filename, cmode, MPI_INFO_NULL, ncid);
        NC_ERR

        MPI_Offset global_nx = nx * client_num;
        MPI_Offset global_ny = ny;

        ierr = ncmpi_def_dim(*ncid, "X", global_nx, &dimid[0]);
        NC_ERR

        ierr = ncmpi_def_dim(*ncid, "Y" ,global_ny, &dimid[1]);
        NC_ERR

        ierr = ncmpi_def_var(*ncid, varname, NC_DOUBLE, 2, dimid, varid);
        NC_ERR

        ierr = ncmpi_enddef(*ncid);
        NC_ERR
       
        ierr = ncmpi_begin_indep_data(*ncid);
        NC_ERR


    }
    if(proc_type == proc_client || proc_type == proc_server){
        MPI_Bcast(ncid, 1, MPI_INT,client_num, server_and_client_comm);
        MPI_Bcast(varid,1, MPI_INT,client_num, server_and_client_comm);
    }
    if(proc_type == proc_client){
        msg ms = {rank,*ncid,*varid,*nwin,nx,ny};
        push_queue(&msg_buffer,ms);

    }
}

void client_work(){
    
    if(proc_type != proc_client) return;

    int rank;
    while(!is_empty_queue(msg_buffer))
    {
        msg ms = front_queue(&msg_buffer);
        pop_queue(&msg_buffer);
        MPI_Send(&ms,1,message,0,0,MPI_COMM_WORLD);
    }
}


void master_work(){
    if(proc_type != proc_master) return ;

    int i;
    int *tags = (int *)malloc(sizeof(int)*server_num);
    while(true)
    {
        msg ms;
        MPI_Status status;
        MPI_Recv(&ms,1,message,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        int rk = ms.rank;
        bool is_empty = is_empty_queue(queue);
        if(get_proc_type(rk) == proc_client)
        {
            push_queue(&queue,ms);
            now_request ++ ;
        }

        else if(get_proc_type(rk) == proc_server)
        {
            msg new_ms;
            if(is_empty) new_ms.rank = -1;
            else 
            {
                new_ms = front_queue(&queue);
                pop_queue(&queue);
            }
            MPI_Send(&new_ms,1,message,ms.rank, tags[rk-client_num - master_num] ,MPI_COMM_WORLD);
            tags[rk - client_num - master_num] ++ ;

        }

        if( is_empty && (now_request == tot_request) ) break;
    }

    for(i = 0;i<server_num;i++){
        msg ms = {-2,0,0,0,0,0};
        MPI_Send(&ms, 1, message, i+client_num+1,tags[i] , MPI_COMM_WORLD);
        tags[i] ++ ;

    }
    free(tags);
}

void server_work(){
    
    if( proc_type != proc_server ) return ;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int *tags = (int *)malloc(sizeod(int)*proc_num);
    for(i=0; i<proc_num; i++) tags[i] = 0;
    while(true)
    {
        int ierr, i, tx, ty;
        double *tmp;
        MPI_Offset  start[2],count[2];
        msg ms;
        MPI_Status status;

        msg ms_send = {rank,0,0,0,0,0};
        MPI_Send(&ms_send,1,message,0,0,MPI_COMM_WORLD);
        ierr = MPI_Recv(&ms,1,message, 0, origin_tag,MPI_COMM_WORLD,&status);
        origin_tag ++ ;

        if(ms.rank==-2 )  break;
        if(ms.rank==-1)  continue;

        tx = ms.nx,ty = ms.ny;
        tmp =(double*)malloc(tx*ty*sizeof(double));
        int rk = ms.rank;
        int ncid = ms.ncid;
        int varid = ms.varid;
        start[0] = tx*(rk-master_num);
        start[1] = 0;
        count[0] = tx;
        count[1] = ty;

        ierr =  MPI_Win_lock(MPI_LOCK_SHARED, rk, 0, ms.nwin);
        ierr = MPI_Get(tmp, tx*ty, MPI_DOUBLE, rk, 0, tx*ty, MPI_DOUBLE, ms.nwin);
        ierr = MPI_Win_unlock(rk, ms.nwin);

        ierr = ncmpi_put_vara_double(ncid, varid, start, count, tmp);
        NC_ERR

    }

}

void dmio_work(){
    client_work();
    master_work();
    server_work();
}
void close_file(int ncid){
    
    int ierr;
    if(proc_type == proc_server){
      
       ierr = ncmpi_end_indep_data(ncid);
       NC_ERR

       ierr = ncmpi_close(ncid);
       NC_ERR
    }
}

void dmio_finalize()
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    printf("rank %d io ended\n",rank);
    destory_queue(&queue);
    destory_queue(&msg_buffer);
}
