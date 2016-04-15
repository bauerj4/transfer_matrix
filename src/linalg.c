#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

/*
  Compute the maximal eigenvalue with a
  power iteration.
*/

double Power_Iteration(double ** mat, int N)
{
  int i;
  double lambda_max, f_old_norm, f_new_norm, diff;
  double * result_vector;

  diff = 1. / 1.e-127;  // Large number

  f_old = VectorMalloc(N);
  f_new = VectorMalloc(N);
  result_vector = VectorMalloc(N);

  // Guess a normalized vector of ones
  for (i=0; i < N; i++)
    f_old[i] = 1;

  f_old_norm = Norm(f_old, N);
  for (i=0; i < N; i++)
    f_old[i] /= f_old_norm;


  // main loop, terminate upon convergence

  while (diff > ITERATION_TOLERANCE)
    {
      diff = 0.;
      MPI_MatrixMultiplyToVector(LocalTransferMatrix, f_old, N);
      f_old_norm = Norm(f_old,N);
      for (i=0; i<N; i++)
	{
	  f_old[i] /= f_old_norm;
	  //printf("f_old[%d] = %f\n", i, f_old[i]);
	  diff += fabs(f_old[i] - f_new[i]) * fabs(f_old[i] - f_new[i]); 
	  //printf("diff = %f\n", diff);
	  f_new[i] = f_old[i];
          //printf("f_new[%d] = %f\n", i, f_new[i]);
	}
      //printf("diff = %f\n", diff);

    }

  for (i=0; i<N; i++)
    result_vector[i] = f_new[i];

  f_new_norm = Norm(f_new, N);
  MPI_MatrixMultiplyToVector(LocalTransferMatrix, result_vector, N);
  lambda_max = Dot(f_new, result_vector,N);
  lambda_max /= (f_new_norm * f_new_norm);

  return lambda_max;
}


/*
  Compute the Euclidean norm of a vector
*/

double Norm(double * a,int N)
{
  double norm;
  norm = Dot(a,a,N);
  norm = pow(norm, 0.5);
  return norm;
}

/*
  Multiply a matrix into a vector from a local copy
*/

void MPI_MatrixMultiplyToVector(double ** mat, double * vec, int N)
{
  int i, rowmin, rowmax, sendcount;
  int * recvcounts;
  double ThisElement;
  double * MyVector;


  /*
    Determine how many elements to receive from each task
  */

  recvcounts = malloc(NTasks * sizeof(int));

  for (i=0; i < NTasks; i++)
    {
      if (i != NTasks - 1)
	{
	  recvcounts[i] = ProcBoundaries[i+1] - ProcBoundaries[i];
	}
      else
	{
	  recvcounts[i] = TransferCount - ProcBoundaries[i];
	}
    }

  /*
    Determine how many elements to send
  */

  rowmin = ProcBoundaries[ThisTask];
  if (ThisTask != NTasks - 1)
    rowmax = ProcBoundaries[ThisTask + 1];
  else
    rowmax = TransferCount - ProcBoundaries[ThisTask];

  MyVector = VectorMalloc(recvcounts[ThisTask]);//VectorMalloc(rowmax - rowmin);

  //PrintMatrix(mat, recvcounts[ThisTask], TransferCount);

  // Get rowspan elements by exploiting the 1D partition
  for (i=0; i < /*rowmax - rowmin*/ recvcounts[ThisTask]; i++)
    {
      ThisElement = Dot(mat[i], vec, N);
      //printf("ThisElement = %f\n", ThisElement);
      MyVector[i] = ThisElement;
    }
  
  //printf("(min, max) on thread %d = %d, %d\n", ThisTask, rowmin, rowmax);
  sendcount = recvcounts[ThisTask];//rowmax - rowmin;
  //printf("sendcount[%d] = %d\n", ThisTask, sendcount);

  /*
  for (i=0; i < NTasks; i++)
    {
      printf("recvcounts[%d] = %d\n", i, recvcounts[i]);
      //printf("sendcount[%d] = %d\n", ThisTask, sendcount);
      }*/
  MPI_Allgatherv(MyVector, sendcount, MPI_DOUBLE, vec, recvcounts, ProcBoundaries, MPI_DOUBLE, MPI_COMM_WORLD);

  //for (i=0; i < N; i++)
  //  printf("Vector[%d] = %f\n",i, vec[i]);
  VectorFree(MyVector, sendcount);
}


/*
  Just the dot product between two vectors
*/

double Dot(double * a, double * b, int N)
{
  int i;
  double dot = 0;
  for (i=0; i<N; i++)
    {
      dot += a[i]*b[i];
    }
  return dot;
}


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


void PrintMatrix(double ** matrix, int n, int m)
{
  int i,j;
  double element;
  printf("Matrix on Task %d: \n", ThisTask);
  for (i=0; i<n; i++)
    {
      for (j=0; j<m; j++)
        {
          element = matrix[i][j];

          if (j == 0)
            {
              printf("[%10.10f, ", element);
            }
          else if (j == m - 1)
            {
              printf(",%10.10f]\n", element);
            }
          else
            {
              printf("%10.10f, ", element);
            }
        }
    }
}
