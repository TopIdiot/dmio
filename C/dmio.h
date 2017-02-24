#ifndef _DMIO_H
#define _DMIO_H
#include "mpi.h"

#define is_free 0
#define is_busy 1

int queue_status = is_free;
int io_status = is_busy;
int master_status = is_free;
int server_status = is_busy;

int tot_request;
int now_request;

void set_status();
void dmio_init();
void close_file(int ncid);
void dmio_save(char *filename,char *varname,double **A, int nx, int ny,int *ncid, int *varid);
void dmio_finalize();
#endif
