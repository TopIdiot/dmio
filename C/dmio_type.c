#include <stdio.h>
#include "dmio_type.h"
int get_proc_type(int rank)
{
    if(rank == 0) return proc_master;
    else if(rank <= client_num) return proc_client;
    return proc_server;
}
