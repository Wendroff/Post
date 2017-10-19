#include<iostream>
#include"MPI_header.h"

int myid,numprocs;
int namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];

void Init(int argc, char* argv[]){
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Get_processor_name(processor_name,&namelen);
};


void Close(){
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (myid == 0)
		std::cout<<"Job Done!"<<std::endl;
	MPI_Finalize();
	
};