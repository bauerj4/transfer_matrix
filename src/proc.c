#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/transfer_options.h"
#include <mpi.h>

void ProcessorPartition1D(int * partition_arr, int N)
{
  int n,nremainder, mymin;
  n = N/NTasks;
  nremainder = N % NTasks;

  if (ThisTask < nremainder)
    {
      mymin = ThisTask * (n + 1);
    }
  else
    {
      mymin = n * ThisTask + nremainder;
    }

  printf("I am thread %d / %d and my min is %d\n", ThisTask + 1, NTasks, mymin);
  MPI_Allgather(&mymin, 1, MPI_INT, partition_arr, 1, MPI_INT, MPI_COMM_WORLD);
}
