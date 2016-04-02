#include "../include/allvars.h"


int ThisTask;
int NTasks;
int ByteCount;
int TransferCount;
int GhostRowUp;
int GhostRowDown;
int * ProcBoundaries; // Array of processor boundaries

double Hamiltonian;
double ** LocalTransferMatrix; // Local portion of TM. Contains 2 ghost rows 

double lambda_max;
double * f_old; // old iteration vector. initially f0 
double * f_new; // new iteration vector  

