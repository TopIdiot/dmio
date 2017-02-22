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

extern int proc_num
extern int client_num
extern int server_num

extern MPI_Group  group
extern MPI_Group only_server_group

int get_proc_type(int &rank)
{
    if(rank==0) return proc_master;
    else if(rank <= client_num) return proc_client
    else return proc_server
}

#endif
