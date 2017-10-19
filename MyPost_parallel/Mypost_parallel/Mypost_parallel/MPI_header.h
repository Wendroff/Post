#ifndef MPI_HEARER_H
#define MPI_HEARER_H
#endif
#include<mpi.h>



extern int myid,numprocs;
extern int namelen;
extern char processor_name[MPI_MAX_PROCESSOR_NAME];

void Init(int argc, char** argv);
void Close();