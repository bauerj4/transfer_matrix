#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/transfer_options.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Init()
{
  int i,j, term;

  MPI_Comm_size(MPI_COMM_WORLD, &NTasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &ThisTask);

  if (ThisTask == 0)
    printf("Initialized %d threads with all communication channels open.\n", NTasks);

  TransferCount = (int)pow(2, MATRIX_SIZE);

  ProcBoundaries = malloc(NTasks * sizeof(int));
  ProcessorPartition1D(ProcBoundaries, TransferCount);
  
  /*
    Now set each thread's ghost rows
  */
  

  if (ThisTask == 0)
    {
      GhostRowUp = TransferCount - 1;
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
    {
      //LocalTransferMatrix = MatrixMalloc(ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask] + 1, TransferCount + 1);
      LocalTransferMatrix = MatrixMalloc(ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask], TransferCount);
    }
  else
    {
      //LocalTransferMatrix = MatrixMalloc(TransferCount -  ProcBoundaries[ThisTask] + 1, TransferCount + 1);
      LocalTransferMatrix = MatrixMalloc(TransferCount -  ProcBoundaries[ThisTask], TransferCount);
    }
  /*
    Now assign values to the elements based on Hamiltonian calculations
  */

  if (ThisTask != NTasks - 1)
    {
      //term = ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask] + 1;
      term = ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask];
    }
  else
    {
      //term = TransferCount - ProcBoundaries[ThisTask] + 1;
      term = TransferCount - ProcBoundaries[ThisTask];
    }

  

  // Assign all values to 1 for debugging
  for (i=0; i<term; i++) // Allocate to ghost row down
    for (j=0; j < TransferCount; j++) // Allocate to ghost column right
      {
	if (fabs((i + ProcBoundaries[ThisTask]) - j) <= 1)
	  LocalTransferMatrix[i][j] = 1;
	else
	  LocalTransferMatrix[i][j] = 0;
      }

  //PrintMatrix(LocalTransferMatrix, term-1, TransferCount);
    
}
