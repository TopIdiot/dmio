/*
 * =====================================================================================
 *
 *       Filename:  dmio_type.h
 *
 *    Description:  Define types used in dmio
 *
 *        Version:  1.0
 *        Created:  2017年02月22日 10时23分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chen Yuwen
 *   Organization:  WXSC
 *
 * =====================================================================================
 */
#ifndef _DMIO_TYPE_H
#define _DMIO_TYPE_H
#include "mpi.h"

#define proc_master 0
#define proc_client 1
#define proc_server 2

int proc_num;
int client_num;
int server_num;
int proc_type;
MPI_Group  group;
MPI_Group only_server_group;
MPI_Group server_and_master_group;
MPI_Group server0_and_client_group;

MPI_Comm only_server_comm;
MPI_Comm server_and_master_comm;
MPI_Comm server0_and_client_comm;

MPI_Win nwin;

int get_proc_type(int rank);

#endif
