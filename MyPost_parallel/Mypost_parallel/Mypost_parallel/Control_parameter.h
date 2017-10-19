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
	string suffix;//�����ļ��ĺ�׺��.dat   _field.dat   _aver.dat
private:
	ifstream control_file;
	unsigned file_start,file_end,file_n_interval;
	unsigned read_time;//��¼�ǵڼ��ζ�д������
	//bool field_data; //��������������.dat ���� _field.dat
	
	void read(string variable_name,unsigned line_num);
};