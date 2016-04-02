#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <stdlib.h>

/*
  Allocate memory to an N X M matrix
*/
double ** MatrixMalloc(int N, int M)
{
  int i;
  double ** mat = malloc(N * sizeof(double*));
  ByteCount += N * sizeof(double *);
  for (i=0; i<N; i++)
    {
      mat[i] = malloc(M * sizeof(double));
      ByteCount += M * sizeof(double);
    }
  return mat;
}

/*
  Free an N X M matrix
*/

void MatrixFree(double ** mat, int N, int M)
{
  int i;

  for (i=0; i < N; i++)
    {
      free(mat[i]);
      ByteCount -= M * sizeof(double);
    }

  ByteCount -= N * sizeof(double *);

  free(mat);
}


/*
  Allocate memory to an N-dim vector
*/

double * VectorMalloc(int N)
{
  double * vec = malloc(N * sizeof(double));
  ByteCount += N * sizeof(double);
  return vec;
}

/*
  Free an N-dim vector
*/

void VectorFree(double * vec, int N)
{
  ByteCount -= N * sizeof(double);
  free(vec);
}
