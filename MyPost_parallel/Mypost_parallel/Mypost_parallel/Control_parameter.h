#ifndef CONTROL_PARAMETER_H
#define CONTROL_PARAMETER_H
#endif

#include<string>
#include<fstream>
//#include"MPI_header.h"

using namespace std;

class Control_parameter{
public:
	//Contrl_parameter();
	void Init();
	string Local_filename();
	unsigned proc_num;
	string suffix;//处理文件的后缀：.dat   _field.dat   _aver.dat
private:
	ifstream control_file;
	unsigned file_start,file_end,file_n_interval;
	unsigned read_time;//记录是第几次读写数据了
	//bool field_data; //用来表明数据是.dat 还是 _field.dat
	
	void read(string variable_name,unsigned line_num);
};