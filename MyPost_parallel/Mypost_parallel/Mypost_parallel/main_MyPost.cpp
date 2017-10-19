/*This program is used to post-porcesser the channel flow data created by hpMusic.
  Built by Zhefu Wang 2017-2-27

  The main jod this code can do is to convert the data form unstructure mesh to structure mesh

  */
//#include<windows.h>
//#include<stdlib.h>
//#include<iostream>
//#include <sys/timeb.h>
//#include<ctime>
//#include<cstdlib>

#include<fstream>
#include<string>
#include<stdexcept>
#include<sstream>
#include<mpi.h>
#ifdef __linux__
#include<unistd.h>
#endif
#include "Point.h"
#include "CPR_data.h"
#include "MPI_header.h"
#include "Control_parameter.h"
using namespace std;


int main(int argc, char* argv[]){
// MPI initialize

	Init(argc,argv);
	
	Control_parameter control_in;
	try{
		control_in.Init();
	} catch (runtime_error err){
		cout<<err.what()<<endl;
		return -1;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	#ifdef __linux__
		sleep(20);
	#endif
	
	//string filename = argv[1];
	string filename = control_in.Local_filename();
	//cout<<filename<<endl;
	while(filename != "over"){
		
		cout<<"Process "<<myid<<" is loading "<<filename<<endl;
		CPR_data test;
		try{
			for(unsigned i=0; i<control_in.proc_num;++i){
				stringstream ss;
				ss<<filename<<"_proc"<<i<<".dat";
				string filename_each = ss.str();
				try{
					test.load(filename_each);
				} catch(bad_alloc &memExp) {
					cout<<memExp.what()<<endl;
					cout<<"Fail on proccess: "<<myid<<endl;
					return -1;
				}
			}
			//test.load("test.dat");
			//test.load("sol_tec_650000.dat_proc0.dat");
			//test.load("sol_tec_650000.dat_proc1.dat");
		} catch (runtime_error err){
			cout<<err.what()<<endl;
			//system("pause");
			return -1;
		}
		//MPI_Barrier(MPI_COMM_WORLD);
		#ifdef __linux__
			sleep(20);
		#endif
		//test.show();
		ostringstream ss;
		int pos = filename.find("."); 
		//int pos = filename.find(".");
		if (control_in.suffix != "_aver.dat"){
			ss<<filename.substr(0,pos)<<"_structure.dat";
			test.structure_output(ss.str());

			ss.str("");//Çå¿Õ
			ss<<filename.substr(0,pos)<<"_averageXZ.dat";
			test.average_xz(ss.str());
		} else {
			try{
				ss.str("");//Çå¿Õ
				ss<<filename.substr(0,pos)<<"_averageXZ.dat";
				test.average_xz_averonly(ss.str());
			} catch (runtime_error err){
				cout<<err.what()<<endl;
				//system("pause");
				return -1;
			}
		}

		//ss.str("");//Çå¿Õ
		//ss<<filename.substr(0,pos)<<"_structure.plt";
		//test.binary_structure_output(ss.str());

		//system("pause");
		
		filename = control_in.Local_filename();
	}
	cout<<"Jobs on Process "<<myid<<" is over!"<<endl;
	MPI_Barrier(MPI_COMM_WORLD);
	#ifdef __linux__
		sleep(20);
	#endif
	//MPI_Barrier(MPI_COMM_WORLD);
	
	Close();
	return 0;
}