#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "mpi.h"
#include "dmio.h"
#include <stdlib.h>
#include "dmio_type.h"
#include "queue.h"
//queue_status 0 empty 1 not empty
//io_status 0 io end  1 io not end 



void set_status()
{
    queue_status = is_free;
    io_status = is_busy;
    master_status = is_free;
    server_status = is_free;
    tot_request = 0;
    now_request = 0;
}

void create_msg_type()
{
    int blocklength[] = {1,1,1,1};
    MPI_Aint offsets[] = {0,sizeof(MPI_INT),2*sizeof(MPI_INT),3*sizeof(MPI_INT)};
    MPI_Datatype oldtypes[] = {MPI_INT,MPI_INT,MPI_INT,MPI_INT};
    MPI_Datatype message;
    MPI_Type_struct(4,blocklength,offsets,oldtypes,&message);
    MPI_Type_commit(&message);
}

void dmio_init()
{
    int rank,size,proc_type;
    int *ranks1;
    int *ranks2;
    int *ranks3;

    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    proc_type = get_proc_type(rank);
    server_num = size - 1 - client_num;
    ranks1 = malloc(sizeof(int)*size);
    ranks2 = malloc(sizeof(int)*size);
   
    for(int i = 0;i<server_num;i++) ranks1[i] = i + client_num + 1;
   
    ranks2[0] = 0;
    for(int i = 1;i<=server_num;i++) ranks2[i] = i + client_num;
   
    ranks3[0] = client_num + 1;
    for(int i=1;i<=client_num;i++) ranks3[i] = i;

    MPI_Comm_group(MPI_COMM_WORLD,&group);
    MPI_Group_incl(group, server_num, ranks1, &only_server_group);
    MPI_Comm_create(MPI_COMM_WORLD, only_server_group, &only_server_comm);

    MPI_Group_incl(group, server_num+1, ranks2, &server_and_master_group);
    MPI_Comm_create(MPI_COMM_WORLD, server_and_master_group, &server_and_master_comm); 

    MPI_Group_incl(group, client_num+1, ranks3, &server0_and_client_group); 
    MPI_Comm_create(MPI_COMM_WORLD,server0_and_client_group,&server0_and_client_comm);
    free(ranks1);
    free(ranks2);
}

void dmio_save(char* filename, char* varname, double **A, int nx, int ny, int *ncid){
    int rank,proc_type;
    MPI_Win nwin;
    tot_request += client_num;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    proc_type = get_proc_type(rank);


    int blocklength[] = {1,1,1,1,1};
    MPI_Aint offsets[] = {0,sizeof(MPI_INT),2*sizeof(MPI_INT),3*sizeof(MPI_INT),4*sizeof(MPI_INT)};
    MPI_Datatype oldtypes[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype message;
    MPI_Type_struct(5,blocklength,offsets,oldtypes,&message);
    MPI_Type_commit(&message);


    if(proc_type == client_type){
        MPI_Win_create(&A,sizeof(double),nx*ny,MPI_INFO_NULL,MPI_COMM_WORLD,&nwin);
    }
    
    else if(proc_type == server_type){
        /* 
         *
         *create nc_file
         *def
         * */


    }
    if(proc_type == client_type || proc_type == server_type){
        MPI_Bcast(ncid,1,MPI_INT,client_num+1,server_and_client_comm);
    }
    if(proc_type == client_type){
        msg ms = {rank,*ncid,nwin,nx,ny};
        MPI_Send(ms,1,message,0,0,MPI_COMM_WORLD);

    }

    else if(master_status == is_free) {

        LinkQueue queue;
        init_queue(&queue);
        master_status = 1;
        while(true)
        {
            msg ms;
            MPI_Status status;
            MPI_Recv(&ms,1,message,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            int rk = ms.rank;
            if(get_proc_type(rk) == proc_client)
            {
                push_queue(&queue,ms);
                now_request ++ ;
            }
            else if(get_proc_type(rk) == proc_server)
            {
                msg new_ms;
                bool is_empty = is_empty_queue(queue);

                if(is_empty)
                    new_ms.rank = -1;

                else 
                {
                    new_ms = front_queue(&queue);
                    pop_queue(&queue);
                }
                MPI_Send(&new_ms,1,message,ms.rank,0,MPI_COMM_WORLD);
            }
            bool is_empty = is_empty_queue(queue);
            if( is_empty && (now_request == tot_request) ) break;
        }
        master_status = 0;
    }

}
void server_work(){
    int blocklength[] = {1,1,1,1,1};
    MPI_Aint offsets[] = {0,sizeof(MPI_INT),2*sizeof(MPI_INT),3*sizeof(MPI_INT),4*sizeof(MPI_INT)};
    MPI_Datatype oldtypes[] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Datatype message;
    MPI_Type_struct(5,blocklength,offsets,oldtypes,&message);
    MPI_Type_commit(&message);
    if(server_status == 0)
    {
        server_status = 1;
        while(true)
        {

            msg ms;
            MPI_Status status;
            MPI_Recv(&ms,1,message, proc_master, MPI_ANY_TAG,MPI_COMM_WORLD,&status);
            if(ms.rank==-2) break;
            double **tmp;
            int tx = ms.nx,ty = ms.ny;
            tmp = malloc(tx*sizeof(double));
            for(int i = 0; i< tx ;i++)
            {
                tmp[i] = malloc(ty*sizeof(double));
            }
            MPI_Get(&tmp,tx*ty,MPI_DOUBLE,ms.rank,0,tx*ty,MPI_DOUBLE,ms.nwin);

        }
    }
    server_status = 0;

}

void close_file(int ncid){
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(get_proc_type(rank) == server_type){
       // ncmpi_close(ncid);
    }
}
void dmio_finalize()
{
    destory_queue(&queue);
}
int main(int argc ,char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Finalize();
}
