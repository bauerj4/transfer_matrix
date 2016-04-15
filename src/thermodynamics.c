#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <stdlib.h>
#include <stdio.h>

/*
  For a sequence of spins, calculate the interaction energy
  between two strips in the lattice
*/


double IsingInteraction(double * spin1, double * spin2, double action, double applied_field)
{
  int i;
  double sum_interaction;

  sum_interaction=0.;
  for (i=0; i < TransferCount; i++)
    {
      sum_interaction -= spin1[i] * spin2[i];
    }

  for (i=0; i < TransferCount - 1; i++)
    {
      sum_interaction -= action * spin1[i] * spin1[i+1];
      sum_interaction -= applied_field * spin1[i];
    }

  /*
    Add ghost row terms to impose BCs
  */

  return sum_interaction;
}

/*
  Generate the ith of 2^N permutations of N half spins which 
  becomes the ith row in the 1D partitioned spin matrix 

  Works up to size 3... need to check what's going wrong.  
  Will write the rest assuming I know the transfer matrix
*/

void GeneratePermutation()
{
  int j;
  int k;
  double * permutation_array = malloc(sizeof(double) * MATRIX_SIZE);
  MaxPermutations = 0;
  /*
    Heap's Algorithm, N times
    Start by saying, "What is every possible permutation of
    k spin ups and N - k spin downs" for all k. Add each
    such permutation to the spin matrix.
  */

  HeapCounter = 0;
  for (k=0; k <= MATRIX_SIZE; k++)
    {
      for (j=0; j < MATRIX_SIZE; j++)
	{
	  if (j < k)
	    {
	      permutation_array[j] = 1;
	    }
	  else
	    {
	      permutation_array[j] = -1;
	    }
	  
	}

      // Now do Heap's algorithm on permutation_array
      HeapRecursive(MATRIX_SIZE, permutation_array);
    }

  
  free(permutation_array);

}


/*
  Check to see if this permutation was already discovered
*/
int PermutationAlreadyFound(double * arr)
{
  int i,j;
  int wasFound = 0;

  i = 0;
  j = 0;
  //printf("Checking for repeats..\n");
  while (i < HeapCounter)
    {
      //printf("Heap Counter = %d\n", HeapCounter);
      while (arr[j] == LocalSpinMatrix[i][j] && j < MATRIX_SIZE)
	{
	  //printf("arr[j] = %f\n", arr[j]);
	  //printf("i,j = %d,%d\n", i, j);
	  j++;
	}
      //printf("i,j = %d,%d\n", i, j);
      if (j == MATRIX_SIZE)
	{
	  wasFound = 1;
	  //printf("Found repeat permutation\n");
	 
	  /*
	  printf("repeat:\n");
	  int k;
	  for (k=0; k < MATRIX_SIZE; k++)
	  printf("%f = %f\n", LocalSpinMatrix[i][k], arr[k]);*/
	}
      j=0;
      i++;
    }
  return wasFound;
}


void HeapRecursive(int n, double * arr)
{
  int i,j;
  // double * new_arr;

  if (n == 1 && PermutationAlreadyFound(arr) == 0)
    {
      for (j=0; j<MATRIX_SIZE; j++)
	{
	  LocalSpinMatrix[HeapCounter][j] = arr[j];
	}
      MaxPermutations++;
      HeapCounter++;
    }
  else if (n == 1)
    {
      MaxPermutations++;
    }
  else
    {
      for (i=0; i<n; i++)
	{
	  HeapRecursive(n-1, arr);
	  if (n % 2 == 1)
	    {
	      Swap(&arr[0], &arr[n-1]);
	    }
	  else
	    {
	      Swap(&arr[i], &arr[n-1]);
	    }
	}
    }
}


void Swap(double * a, double * b)
{
  double temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

