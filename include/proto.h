/*
  This file contains function prototypes
  for the transfer matrix analysis.
*/

/*
  Initialization and memory routines
*/


double ** MatrixMalloc(int N, int M);
void  MatrixFree(double ** mat, int N, int M);
double * VectorMalloc(int N);
void VectorFree(double * vec, int N);

void Init();
void ProcessorPartition1D(int * partition_arr, int N);
void Finalize();
