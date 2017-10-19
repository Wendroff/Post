#ifndef CPR_DATA_H
#define CPR_DATA_H

#include<string>
#include<map>
#include<vector>
#include"Point.h"
#include"Y_height.h"
using namespace std;
class CPR_data{
public:
//	CPR_data(string a):filename(a){};
	void load(string filename);
	void show();
	void structure_output(string output_file);
	void binary_structure_output(string output_file);
	void average_xz(string output_file);
	void average_xz_averonly(string output_file);
	//~CPR_data();
private:
//	string             filename;//�ļ���
	map<Point,vector<double>> data;    //��������
	map<Y_height,vector<double>> data_avg;//��������x zƽ��֮�������

	string             first_line; //�ļ�ͷ�ĵ�һ�У�Variable��һ�У�,���ԱȽϺ���������ļ��еı����Ƿ�һ�¡�
	vector<string>     var_name; //������
	double             solution_time; //������������ʱ�� 
	unsigned           n_var; //������
//	unsigned           n_node;//��ǰ�ļ��ĵ�����������ļ��Ĳ�ͬ����

};



#endif