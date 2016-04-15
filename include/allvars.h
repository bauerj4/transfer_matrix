/*
  Protoypes for all external global variables.  These
  may be defined anywhere, but almost always in 
  allvars.c.  
*/


extern int ThisTask;
extern int NTasks;
extern int ByteCount;
extern int ByteCount_G;
extern int TransferCount;
extern int GhostRowUp;
extern int GhostRowDown;
extern int * ProcBoundaries; // Array of processor boundaries

extern double Hamiltonian;
extern double ** LocalSpinMatrix; // Matrix of every possible spin configuration
extern double ** LocalTransferMatrix; // Local portion of TM. Contains 2 ghost rows
extern int HeapCounter;
extern int MaxPermutations;

extern double lambda_max;
extern double * f_old; // old iteration vector. initially f0
extern double * f_new; // new iteration vector

