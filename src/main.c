#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <mpi.h>


/*
  This program uses distributed-memory power iterations to compute 
  statistical averages for thermodynamic quantities from 2D transfer
  matrices.  It is submitted for partial fulfillment of the requirements
  of PHYS 870 at Queen's University, Winter 2016.

  Copyright J.S. Bauer
*/


int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv); // Open all communication channels
  Init();
  
  TemperatureSweep();

  Finalize();
  MPI_Finalize(); // Finalize
  return 0; // Success
}
