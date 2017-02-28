#ifndef _DMIO_H
#define _DMIO_H
#include "mpi.h"

#define is_free 0
#define is_busy 1

int queue_status ;
int io_status;
int master_status;
int server_status;

int tot_request;
int now_request;

void set_status();
void dmio_init();
void dmio_work();
void server_work();
void master_work();
void client_work();
void close_file(int ncid);
void dmio_save(char *filename,char *varname, double *A, int nx, int ny, int *ncid, int *varid, MPI_Win *win);
void dmio_laod(chr *filename, char *varname, double *A, int nx, int ny);
void dmio_finalize();
#endif
