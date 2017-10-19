#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<iomanip>

#include"CPR_data.h"
#include "TECIO.h"
#include "MASTER.h" /* for defintion of NULL */

using namespace std;

void CPR_data::load(string filename){
	ifstream input(filename);
	if(!input.is_open()){
		cout<<"Can not open "<<filename<<endl;
		throw runtime_error("Can not open the input file!");
	}
	cout<<"Loading data form "<<filename<<endl;

	string line;//用来读取每一行的数据
	string::size_type    pos;//储存分隔符的位置
	getline(input,line);
	if (first_line.empty()){
		first_line = line;
		pos = line.find("=");
		if (pos>=line.size())
			throw runtime_error("There is someting wrong with line 1 !");
		line.erase(0,pos+1);

		while(!line.empty()){
			pos = line.find(",");
			if (pos>=line.size()){
				var_name.push_back(line);
				var_name.push_back("n_repeat");
				break;
			}
			var_name.push_back(line.substr(0,pos));
			line.erase(0,pos+1);
		}
		auto iter = var_name.begin();
		if (*iter != "x"){
			throw runtime_error("The first variable is not x!");
		} else {
			iter++;
			if (*iter != "y"){
				throw runtime_error("The second variable is not y!");
			} else {
				iter++;
				if (*iter != "z")
					throw runtime_error("The third variable is not z!");
			}
		}
		n_var = var_name.size();
	} else {
		if (line != first_line){
			throw runtime_error("The first line of this file is not match with others!");
		}
	}
	

	//for(auto iter=var_name.begin(); iter!=var_name.end();iter++)
	//	cout<<*iter<<endl;

	getline(input,line);
	//cout<<line<<endl;
	pos = line.find("SOLUTIONTIME");
	if (pos>=line.size())
		throw runtime_error("There is someting wrong with line 2 !");
	line.erase(0,pos+12);
	//cout<<line<<endl;
	pos = line.find("=");
	if (pos>=line.size())
		throw runtime_error("There is someting wrong with line 2 !");
	line.erase(0,pos+1);
	solution_time = stod(line);
	//cout<<solution_time<<endl;
	
	getline(input,line);
	pos = line.find("n");
	if (pos>=line.size())
		throw runtime_error("There is someting wrong with line 3 !");
	line.erase(0,pos+1);
	//cout<<line<<endl;
	pos = line.find("=");
	if (pos>=line.size())
		throw runtime_error("There is someting wrong with line 3 !");
	line.erase(0,pos+1);
	pos = line.find(",");
	if (pos>=line.size())
		throw runtime_error("There is someting wrong with line 3 !");
	unsigned n_node = stoul(line.substr(0,pos)); //保存该文件中需要读取的点数

	getline(input,line);

	//开始读入数据。注意：数据前三个值必须是x,y,z*************************************************
	double x,y,z;
	//int total_repeat = 0;
	for(unsigned i=0; i<n_node; ++i){
		input>>x;
		input>>y;
		input>>z;
		Point p(x,y,z);
		double *data_ = new double[n_var-3];
		for(unsigned j = 3; j<n_var-1;j++)
			input>>data_[j-3];
		data_[n_var-4] = 1.0;//将n_repeat先置为1
		auto insert_flag = data.insert(make_pair(p,data_));
		if (!insert_flag.second){    //如果该位置的数据已经存在，就进行平均
			double old_n_repeat = (insert_flag.first->second)[n_var-4];
			for(unsigned i=0;i<(n_var-4);++i){
				(insert_flag.first->second)[i] = ((insert_flag.first->second)[i] * old_n_repeat + data_[i])/(old_n_repeat+1);
			}
			(insert_flag.first->second)[n_var-4] += 1.0;
			//++total_repeat;
		}
	}
	//cout<<total_repeat<<endl;
	input.close();


}

void CPR_data::show(){
//	cout<<"file name     = "<< filename<<endl;
	cout<<"n_var         = "<< n_var <<endl;
//	cout<<"n_node        = "<< n_node<<endl;
	cout<<"solution time = "<<solution_time<<endl;
	for(auto iter=var_name.begin(); iter!=var_name.end();iter++)
		cout<<*iter<<"   ";
	cout<<endl;
}

void CPR_data::structure_output(string output_file){
	//先确定Nz Ny Nx
	unsigned Nz = 0;
	unsigned Ny = 0;
	unsigned Nx = 0;
	unsigned temp = 1;
	//unsigned Nz = 161;
	//unsigned Ny = 125;
	//unsigned Nx = 189;
	auto map_it = data.cbegin();
	Point first_point = map_it->first;
	while(map_it != data.cend()){
		++map_it;
		++temp;
		if(!first_point.is_y_same(map_it->first)){
			Nz = temp-1;
			break;
		}
	}
	
	while(map_it != data.cend()){
		++map_it;
		++temp;
		if(!first_point.is_x_same(map_it->first)){
			Ny = temp-1;
			break;
		}
	}
	if ((data.size()%Ny)!=0){
		cout<<"Something wrong with counting Nx!"<<endl;
	} 
	if ((Ny%Nz)!=0){
		cout<<"Nz = "<<Nz<<endl;
		cout<<"Ny*Nz = "<<Ny<<endl;
		cout<<"Something wrong with counting Ny!"<<endl;
	} 
	Nx = data.size()/Ny;
	Ny = Ny/Nz;
	
	
	


	ofstream output(output_file);
//	output<<setiosflags(ios::fixed);
	cout<<"Writing to the file: "<<output_file<<endl;
	output<<setprecision(12);
	output<<"Variables =  ";
	for(auto iter = var_name.begin(); iter != var_name.end(); ++iter){
		output<< *iter <<"  ";
	}
	output<<endl;
	output<<"ZONE F=POINT I = "<<Nz<<"   J = "<<Ny<<"    K = "<<Nx<<endl;
	map_it = data.cbegin();
	while(map_it != data.cend()){
		output<<(map_it->first).getx()<<"  "
			  <<(map_it->first).gety()<<"  "
			  <<(map_it->first).getz()<<"  ";
		for(unsigned i=3; i<n_var; ++i){
			output<<(map_it->second)[i-3]<<"  ";
		}
		output<<endl;
		++map_it;
	}
	output.close();
}

//void CPR_data::binary_structure_output(string output_file){
//	//先确定Nz Ny Nx
//	unsigned Nz = 0;
//	unsigned Ny = 0;
//	unsigned Nx = 0;
//	unsigned temp = 1;
//	//unsigned Nz = 161;
//	//unsigned Ny = 125;
//	//unsigned Nx = 189;
//	auto map_it = data.cbegin();
//	Point first_point = map_it->first;
//	while(map_it != data.cend()){
//		++map_it;
//		++temp;
//		if(!first_point.is_y_same(map_it->first)){
//			Nz = temp-1;
//			break;
//		}
//	}
//	
//	while(map_it != data.cend()){
//		++map_it;
//		++temp;
//		if(!first_point.is_x_same(map_it->first)){
//			Ny = temp-1;
//			break;
//		}
//	}
//	if ((data.size()%Ny)!=0){
//		cout<<"Something wrong with counting Nx!"<<endl;
//	} 
//	if ((Ny%Nz)!=0){
//		cout<<"Nz = "<<Nz<<endl;
//		cout<<"Ny*Nz = "<<Ny<<endl;
//		cout<<"Something wrong with counting Ny!"<<endl;
//	} 
//	Nx = data.size()/Ny;
//	Ny = Ny/Nz;
//	
//	
//	
//
//
//	
//	map_it = data.cbegin();
//	while(map_it != data.cend()){
//		output<<(map_it->first).getx()<<"  "
//			  <<(map_it->first).gety()<<"  "
//			  <<(map_it->first).getz()<<"  ";
//		for(unsigned i=3; i<n_var; ++i){
//			output<<(map_it->second)[i-3]<<"  ";
//		}
//		output<<endl;
//		++map_it;
//	}
//	
//}

void CPR_data::average_xz(string output_file){
	auto map_it = data.cbegin();
	while(map_it != data.cend()){
		Y_height yy((map_it->first).gety());
		auto insert_flag = data_avg.insert(make_pair(yy,map_it->second));
		if(insert_flag.second){//如果成功插入，将n_repeat置为 1
			(insert_flag.first->second)[n_var-4] = 1.0;
		} else {
			double old_n_repeat = (insert_flag.first->second)[n_var-4];
			for(unsigned i=0;i<(n_var-4);++i){
				(insert_flag.first->second)[i] = ((insert_flag.first->second)[i] * old_n_repeat + (map_it->second)[i])/(old_n_repeat+1);
			}
			(insert_flag.first->second)[n_var-4] += 1.0;
		}
		++map_it;
	}

	//开始输出平均文件
	ofstream output(output_file);
//	output<<setiosflags(ios::fixed);
	cout<<"Writing to the file: "<<output_file<<endl;
	output<<setprecision(12);
	output<<"Variables =  y  ";
	for(auto iter = var_name.begin()+3; iter != var_name.end(); ++iter){
		output<< *iter <<"  ";
	}
	output<<endl;
	output<<"ZONE F=POINT I = "<<data_avg.size()<<endl;
	auto map_it_avg = data_avg.cbegin();
	while(map_it_avg != data_avg.cend()){
		output<<(map_it_avg->first).gety()<<"  ";
		for(unsigned i=3; i<n_var; ++i){
			output<<(map_it_avg->second)[i-3]<<"  ";
		}
		output<<endl;
		++map_it_avg;
	}
	output.close();
}
