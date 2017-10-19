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
extern int myid;
void CPR_data::load(string filename){
	ifstream input(filename);
	if(!input.is_open()){
		cout<<"Can not open "<<filename<<endl;
		throw runtime_error("Can not open the input file!");
	}
	//cout<<"Loading data form "<<filename<<endl;

	string line;//������ȡÿһ�е�����
	string::size_type    pos;//����ָ�����λ��
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
	unsigned n_node = stoul(line.substr(0,pos)); //������ļ�����Ҫ��ȡ�ĵ���

	getline(input,line);

	//��ʼ�������ݡ�ע�⣺����ǰ����ֵ������x,y,z*************************************************
	double x,y,z;
	//int total_repeat = 0;
	for(unsigned i=0; i<n_node; ++i){
		input>>x;
		input>>y;
		input>>z;
		Point p(x,y,z);
		
		vector<double> data_(n_var-3,0.0);
		
		for(unsigned j = 3; j<n_var-1;j++)
			input>>data_[j-3];
		data_[n_var-4] = 1.0;//��n_repeat����Ϊ1
		auto insert_flag = data.insert(make_pair(p,data_));
		if (!insert_flag.second){    //�����λ�õ������Ѿ����ڣ��ͽ���ƽ��
			double old_n_repeat = (insert_flag.first->second)[n_var-4];
			for(unsigned i=0;i<(n_var-4);++i){
				(insert_flag.first->second)[i] = ((insert_flag.first->second)[i] * old_n_repeat + data_[i])/(old_n_repeat+1);
			}
			(insert_flag.first->second)[n_var-4] += 1.0;
			//++total_repeat;
		}
		//delete [] data_;
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
	//��ȷ��Nz Ny Nx
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
//	//��ȷ��Nz Ny Nx
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
		if(insert_flag.second){//����ɹ����룬��n_repeat��Ϊ 1
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

	//��ʼ���ƽ���ļ�
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

void CPR_data::average_xz_averonly(string output_file){
	//����ļ��е������ǲ�����ȷ
	if ( ! (var_name[5]  == "Rho_aver" && 
		    var_name[6]  == "U_aver" &&
			var_name[7]  == "V_aver" &&
			var_name[8]  == "W_aver" &&
			var_name[9]  == "P_aver" &&
			var_name[10] == "uu" &&
			var_name[11] == "vv" &&
			var_name[12] == "ww" &&
			var_name[13] == "uv" &&
			var_name[14] == "vw" &&
			var_name[15] == "uw") 
		){
		throw runtime_error(" Variables in the file *_aver.dat is incorrect!");
	}
	//��uu����ŵӦ��ת��ΪUU�ȶ��׾أ����ܽ��мӺ�
	for (auto map_it = data.begin(); map_it != data.end(); ++map_it){
		//cout<<((map_it->second)[0])<<endl;
		double U_aver = (map_it->second)[3];
		double V_aver = (map_it->second)[4];
		double W_aver = (map_it->second)[5];
		(map_it->second)[7]  += U_aver*U_aver;
		(map_it->second)[8]  += V_aver*V_aver;
		(map_it->second)[9]  += W_aver*W_aver;
		(map_it->second)[10] += U_aver*V_aver;
		(map_it->second)[11] += V_aver*W_aver;
		(map_it->second)[12] += U_aver*W_aver;

	}
	//����XZ�ռ�ƽ����map
	auto map_it = data.cbegin();
	while(map_it != data.cend()){
		Y_height yy((map_it->first).gety());
		auto insert_flag = data_avg.insert(make_pair(yy,map_it->second));
		if(insert_flag.second){//����ɹ����룬��n_repeat��Ϊ 1
			(insert_flag.first->second)[n_var-4] = 1.0;
		} else {
			double old_n_repeat = (insert_flag.first->second)[n_var-4];
			for(unsigned i=0;i<(n_var-4);++i){
				(insert_flag.first->second)[i] = ((insert_flag.first->second)[i] * old_n_repeat + (map_it->second)[i])/(old_n_repeat+1);//�˷��˺ö������������������д�������
			}
			(insert_flag.first->second)[n_var-4] += 1.0;
		}
		++map_it;
	}
	//��UU�ȶ��׾�ת��Ϊuu����ŵӦ�������ܽ��мӺ�
	for (auto map_it = data_avg.begin(); map_it != data_avg.end(); ++map_it){
		//cout<<((map_it->second)[0])<<endl;
		double U_aver = (map_it->second)[3];
		double V_aver = (map_it->second)[4];
		double W_aver = (map_it->second)[5];
		(map_it->second)[7]  -= U_aver*U_aver;
		(map_it->second)[8]  -= V_aver*V_aver;
		(map_it->second)[9]  -= W_aver*W_aver;
		(map_it->second)[10] -= U_aver*V_aver;
		(map_it->second)[11] -= V_aver*W_aver;
		(map_it->second)[12] -= U_aver*W_aver;

	}
	//��ʼ���ƽ���ļ�
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
