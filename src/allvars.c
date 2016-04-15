#include "../include/allvars.h"
#include <stdio.h>
#include <stdlib.h>

int ThisTask;
int NTasks;
int ByteCount;
int ByteCount_G;
int TransferCount;
int GhostRowUp;
int GhostRowDown;
int * ProcBoundaries; // Array of processor boundaries

double Temperature;
double DeltaT;
double Hamiltonian;
double * T_arr;
double * Beta_arr;
double * Partition_arr;
double ** LocalSpinMatrix; 
double ** LocalTransferMatrix; // Local portion of TM. Contains 2 ghost rows 
int ThisStep;
int HeapCounter;
int MaxPermutations;

double lambda_max;
double * f_old; // old iteration vector. initially f0 
double * f_new; // new iteration vector  

FILE * fileout;
