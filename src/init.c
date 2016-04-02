#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/transfer_options.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void Init()
{
  MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

  if (ThisTask == 0)
    printf("Initialized %d threads with all communication channels open.\n", NTasks);

  ProcBoundaries = malloc(NTasks * sizeof(int));
  ProcessorPartition1D(ProcBoundaries, MATRIX_SIZE);
  
  /*
    Now set each thread's ghost rows
  */
  
  if (ThisTask == 0)
    {
      GhostRowUp = MATRIX_SIZE - 1;
    }
  else
    {
      GhostRowUp = ProcBoundaries[ThisTask] - 1; // The last row of the last processor
    }

  if (ThisTask == NTasks - 1)
    {
      GhostRowDown = 0;
    }
  
  else
    {
      GhostRowDown = ProcBoundaries[ThisTask] + 1; // The first row of the next processor
    }
  
  /*
    Now allocate the local transfer matrix
  */

  if (ThisTask != NTasks - 1)
    LocalTransferMatrix = MatrixMalloc(MATRIX_SIZE, ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask] + 2);
  else
    LocalTransferMatrix = MatrixMalloc(MATRIX_SIZE,MATRIX_SIZE -  ProcBoundaries[ThisTask] + 2);

  /*
    Now assign values to the elements based on Hamiltonian calculations
  */

}
