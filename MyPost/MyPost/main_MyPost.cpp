/*This program is used to post-porcesser the channel flow data created by hpMusic.
  Built by Zhefu Wang 2017-2-27


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

#include "Point.h"
#include "CPR_data.h"
using namespace std;


int main(int argc, char* argv[]){
	CPR_data test;
	string filename = argv[1];
	unsigned n_proc = stoul(argv[2]);
	if (argc != 3){
		cout<<"The number of input argument is wrong:"<<endl;
		cout<<"MyPost.exe filename_no_proc n_proc"<<endl;
		//system("pause");
		return 1;
	}
	//unsigned Nz = 161;
	//unsigned Ny = 125;
	//unsigned Nx = 189;
	try{
		for(unsigned i=0; i<n_proc;++i){
			stringstream ss;
			ss<<filename<<"_proc"<<i<<".dat";
			string filename_each = ss.str();
			test.load(filename_each);
		}
		//test.load("test.dat");
		//test.load("sol_tec_650000.dat_proc0.dat");
		//test.load("sol_tec_650000.dat_proc1.dat");
	} catch (runtime_error err){
		cout<<err.what()<<endl;
		//system("pause");
		return -1;
	}

	//test.show();
	ostringstream ss;
	auto pos = filename.find("."); 
	//int pos = filename.find(".");
	ss<<filename.substr(0,pos)<<"_structure.dat";
	test.structure_output(ss.str());

	ss.str("");//Çå¿Õ
	ss<<filename.substr(0,pos)<<"_averageXZ.dat";
	test.average_xz(ss.str());

	//ss.str("");//Çå¿Õ
	//ss<<filename.substr(0,pos)<<"_structure.plt";
	//test.binary_structure_output(ss.str());

	//system("pause");
	
	return 0;
}