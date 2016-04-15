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
    Allocate memory to output arrays
  */

  ThisStep = 0;
  T_arr = VectorMalloc(NTEVALS);
  Beta_arr = VectorMalloc(NTEVALS);
  Partition_arr = VectorMalloc(NTEVALS);
  
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
      //LocalSpinMatrix = MatrixMalloc(ProcBoundaries[ThisTask+1] - ProcBoundaries[ThisTask], TransferCount);
      LocalSpinMatrix = MatrixMalloc(TransferCount, MATRIX_SIZE);
    }
  else
    {
      //LocalTransferMatrix = MatrixMalloc(TransferCount -  ProcBoundaries[ThisTask] + 1, TransferCount + 1);
      LocalTransferMatrix = MatrixMalloc(TransferCount -  ProcBoundaries[ThisTask], TransferCount);
      //LocalSpinMatrix = MatrixMalloc(TransferCount -  ProcBoundaries[ThisTask], TransferCount);
      LocalSpinMatrix = MatrixMalloc(TransferCount, MATRIX_SIZE);
    }

  MPI_Allreduce(&ByteCount, &ByteCount_G, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
  if (ThisTask == 0)
    printf("Allocated %10.5f MB of system memory across %d threads.\n", (double)ByteCount_G/1.e6, NTasks);
  /*
    Now assign values to the spin matrix
  */
  GeneratePermutation();

}
