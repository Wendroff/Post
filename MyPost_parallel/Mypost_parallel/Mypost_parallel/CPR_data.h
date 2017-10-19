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
//	string             filename;//文件名
	map<Point,vector<double>> data;    //储存数据
	map<Y_height,vector<double>> data_avg;//储存沿着x z平均之后的数据

	string             first_line; //文件头的第一行（Variable那一行）,用以比较后读进来的文件中的变量是否一致。
	vector<string>     var_name; //变量名
	double             solution_time; //结果输出的物理时间 
	unsigned           n_var; //变量数
//	unsigned           n_node;//当前文件的点数，会根据文件的不同而变

};



#endif