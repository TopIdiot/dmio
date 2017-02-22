#include "mpi.h"
#include "dmio.h"
#include <stdlib.h>
#include "dmio_type.h"
//queue_status 0 empty 1 not empty
//io_status 0 io end  1 io not end 
int queue_status = 0;
int io_status = 1;
void dmio_init()
{
    int rank,size,proc_type;
    int *ranks1;
    int *ranks2;

    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    proc_type = get_proc_type(rank);
    server_num = size - 1 - client_num;
    ranks1 = malloc(sizeof(int)*size);
    ranks2 = malloc(sizeof(int)*size);
   
    for(int i = 0;i<server_num;i++) ranks1[i] = i + client_num + 1;
   
    ranks2[0] = 0;
    for(int i = 1;i<=server_num;i++) ranks2[i] = i + client_num;
   
    MPI_Comm_group(MPI_COMM_WORLD,&group);
    MPI_Group_incl(group, server_num, ranks1, &only_server_group);
    MPI_Comm_create(MPI_COMM_WORLD, server_group, &only_server_comm);

    MPI_Group_incl(group,server_num+1, ranks2, &server_and_master_group);
    MPI_Comm_create(MPI_COMM_WORLD, server_and_master_group, &server_and_master_comm);   
    free(ranks1);
    free(ranks2);
}

void dmio_save(char *filename, char *varname , double ** A, int nx, int ny)
{

    if(proc_type == proc_master)
    {
        if(master_status == 0)
        {
            Linkqueue queue;
            initQueue(&queue);
            master_status = 1;
            while(true)
            {
                msg ms;
                MPI_Recv(ms,1,msg,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD);
                int rk = ms.rank;
                if(get_prco_type(rk) == proc_client)
                {
                    push_queue(&queue,msg);
                }
                else if(get_proc_type(rk) == proc_server)
                {
                    msg new_ms;
                    bool is_empty = empty_queue(&queue);

                    if(is_empty)
                        new_ms.rank = -1;
                   
                    else 
                    {
                        msg new_ms = front_queue(&queue);
                        pop_queue(&queue);
                    }
                    MPI_Send(nwe_ms,1,msg,ms.rank,0,MPI_COMM_WORLD);
                }
                bool is_empty = empty_queue(&queue);
                if(is_empty && !io_status) break;


            }
            master_status = 0;
            destory_queue(&queue);
        }

    }

    else if(proc_type == proc_client)
    {
        MPI_Win_create(&A,sizeof(double),nx*ny,MPI_INFO_NULL,MPI_COMM_WORLD,&nwin);
        msg ms(rk,nx,ny,nwin);
        MPI_Send(ms,1,msg,0,0,MPI_COMM_WORLD);
        

    }
    else 
    {
        if(server_status == 0)
        {
            server_status = 1;
            while(true)
            {

                msg ms;
                MPI_Recv(&ms,1,msg, proc_master, MPI_ANY_TAG,MPI_COMM_WORLD);
                if(ms.rank >=0 ) queue_status = 1;
                else if(ms.rank==-1) queue_status = 0;
                else if(rank==-2) io_stauts = 0;
                double **tmp;
                int tx = ms.nx,ty = ms.ny;
                tmp = malloc(tx*sizeof(double));
                for(int i = 0; i< tx ;i++)
                {
                    tmp[i] = malloc(ty*sizeof(double));
                }
                MPI_Get(&tmp,tx*ty,double,double,ms.rk,0,tx*ty,double,ms.nwin);


                if((io_status == 0) &&(queue_status==0)) break;
            }
            server_status = 0;
        }

    }
}

void close_file(int *ncid)
{}

void io_end()
{
    io_status = 0;
}
int main(int argc ,char **argv)
{
    MPI_Init(&argc, &argv);
    MPI_Finalize();
}
