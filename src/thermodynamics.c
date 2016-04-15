#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/transfer_options.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/*
  Do a sweep over the temperature range, calculating relevant
  quantities and storing them
*/

void TemperatureSweep()
{
  DeltaT = (T_MAX - T_MIN) / ((double)NTEVALS);
  Temperature = T_MIN;
  double ThisLambda = 0.;
  double ThisPart = 0.;

  while (Temperature < T_MAX)
    {
      GenerateTransferMatrix(Temperature);
      ThisLambda = Power_Iteration(LocalTransferMatrix, TransferCount); 
      ThisPart = MATRIX_SIZE * log(ThisLambda);
      printf("Temperature = %5.5f Lambda = %5.5f ThisPart = %5.5f\n", Temperature, ThisLambda, ThisPart);
      T_arr[ThisStep] = Temperature;
      Beta_arr[ThisStep] = 1./(Temperature * BOLTZMANN_CONSTANT);
      Partition_arr[ThisStep] = ThisPart / (MATRIX_SIZE * MATRIX_SIZE);
      ThisStep++;
      Temperature += DeltaT;
    }
}


/*
  Using the permuations of spins, generate every element
  of the transfer matrix.
*/

void GenerateTransferMatrix(double temperature)
{
  int i, j, rowmin, rowmax;
  double interaction, transfer_element,beta;

  rowmin = ProcBoundaries[ThisTask];

  if (ThisTask != NTasks - 1)
    rowmax = ProcBoundaries[ThisTask + 1] - ProcBoundaries[ThisTask];
  else
    rowmax = TransferCount - ProcBoundaries[ThisTask];

  /*
    Now get the elements
  */

  beta = 1. / (temperature * BOLTZMANN_CONSTANT);
  for (i=rowmin; i < rowmax; i++)
    for (j=0; j < TransferCount; j++)
      {
	//printf("i,j = %d, %d\n",i,j);
	interaction = IsingInteraction(LocalSpinMatrix[i], LocalSpinMatrix[j], ACTION, B_FIELD);
	//beta = 1. / (temperature * BOLTZMANN_CONSTANT);
	transfer_element = exp(-beta * interaction);
	//printf("Transfer Element, interaction  = %f, %f\n",transfer_element, interaction);
	LocalTransferMatrix[i][j] = transfer_element;
      }

  //PrintMatrix(LocalTransferMatrix, rowmax - rowmin, TransferCount);
}



/*
  For a sequence of spins, calculate the interaction energy
  between two strips in the lattice
*/


double IsingInteraction(double * spin1, double * spin2, double action, double applied_field)
{
  int i;
  double sum_interaction;

  sum_interaction=0.;

  for (i=0; i < MATRIX_SIZE; i++)
    {
      sum_interaction += spin1[i] * spin2[i];
      sum_interaction += 0.5 * spin1[i] * spin1[(i+1)%MATRIX_SIZE];
      sum_interaction += 0.5 * spin2[i] * spin2[(i+1)%MATRIX_SIZE];
    }

  /*
  for (i=0; i < TransferCount; i++)
    {
      sum_interaction -= spin1[i] * spin2[i];
    }

  for (i=0; i < TransferCount - 1; i++)
    {
      sum_interaction -= action * spin1[i] * spin1[i+1];
      sum_interaction -= applied_field * spin1[i];
      }*/

  /*
    Add ghost row terms to impose BCs
  */

  //printf("Sum Interaction = %f\n", sum_interaction);
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
  while (i < HeapCounter)
    {
      while (arr[j] == LocalSpinMatrix[i][j] && j < MATRIX_SIZE)
	{
	  j++;
	}
      if (j == MATRIX_SIZE)
	{
	  wasFound = 1;
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

