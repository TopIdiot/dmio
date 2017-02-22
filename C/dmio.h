#ifndef _DMIO_H
#define _DMIO_H
#include "mpi.h"

void dmio_init();
void dmio_save(char *filename,char *varname,double **A, int nx, int ny);
#endif
