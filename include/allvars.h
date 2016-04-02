/*
  Protoypes for all external global variables.  These
  may be defined anywhere, but almost always in 
  allvars.c.  
*/


extern int ThisTask;
extern int NTasks;
extern int ByteCount;
extern int GhostRowUp;
extern int GhostRowDown;
extern int * ProcBoundaries; // Array of processor boundaries

extern double Hamiltonian;
extern double ** LocalTransferMatrix; // Local portion of TM. Contains 2 ghost rows


#ifdef POWER_ITERATION
extern double lambda_max;
extern double * f_old; // old iteration vector. initially f0
extern double * f_new; // new iteration vector
#endif 

