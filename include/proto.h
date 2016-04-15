/*
  This file contains function prototypes
  for the transfer matrix analysis.
*/

/*
  Linear algebra routines
*/


double Power_Iteration(double ** mat, int N);
double Norm(double * a,int N);
void MPI_MatrixMultiplyToVector(double ** mat, double * vec, int N);
double Dot(double * a, double * b, int N);

double ** MatrixMalloc(int N, int M);
void  MatrixFree(double ** mat, int N, int M);
double * VectorMalloc(int N);
void VectorFree(double * vec, int N);
void PrintMatrix(double ** mat, int n, int m);


/*
  Initialization
*/

void Init();
void ProcessorPartition1D(int * partition_arr, int N);
void Finalize();


/*
  Thermodynamics
*/

void TemperatureSweep();
void GenerateTransferMatrix(double Temperature);
double IsingInteraction(double * spin1, double * spin2, double action, double applied_field);
void GeneratePermutation();
void HeapRecursive(int n, double * arr);
void LexicographicPermutations(int n, double * arr);
void Swap(double * a, double * b);

/*
  Stats
*/

long int choose(int n, int k);
long int factorial(int n);
