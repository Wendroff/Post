#include<string>
#include<fstream>
#include<iostream>
#include<stdexcept>
#include"Control_parameter.h"
#include"MPI_header.h"

using namespace std;

void Control_parameter::Init(){
	read_time = 0;
	control_file.open("control.in");
	

	if(!control_file.is_open()){
		cout<<"Can not open control.in"<<endl;
		throw runtime_error("Can not open the input file!");
	}
	//cout<<myid<<endl;
	if(myid==0)
		cout<<"Loading control.in "<<endl;
	string line;//用来读取每一行的数据
	string variable_name;//用来存放变量名
	string::size_type    pos;//储存分隔符的位置
	//读取第一行说明，这一行没有什么用
	getline(control_file,line);
	//读取第二到六行 file_start
	//try{
		read("file_start",2);
		read("file_end",3);
		read("file_num_interval",4);
		read("proc_num",5);
		read("suffix",6);
	/*} catch (runtime_error err){
		cout<<err.what()<<endl;
		throw;
	}*/
	return;
};

void Control_parameter::read(string variable_name,unsigned line_num){//参数为变量名和所读取的行数
	string line;//用来读取每一行的数据
	
	string::size_type    pos;//储存分隔符的位置
		
	getline(control_file,line);
	pos = line.find("=");
	if (pos>=line.size())
		throw runtime_error(("Cannot find a = in line "+to_string(line_num)));//
	string read_var_name = line.substr(0,pos);
	//cout<<variable_name<<endl;
	read_var_name.erase(read_var_name.find_last_not_of(" ") + 1); 
	if (read_var_name != variable_name){
		//cout<<variable_name<<endl;
		cout<<"Cannot find "<<variable_name<<endl;
		cout<<variable_name<<" should be in line "<<line_num<<endl;
		throw runtime_error(("Cannot find "+variable_name+" in line "+to_string(line_num)));
	}else{
		//cout<<line.substr(pos,line.length())<<endl;
		if (variable_name == "file_start") {
			file_start = stoul(line.substr(pos+1,line.length()));
			if(myid==0)
				cout<<"file_start = "<<file_start<<endl;
		} else if (variable_name == "file_end") {
			file_end = stoul(line.substr(pos+1,line.length()));
			if(myid==0)
				cout<<"file_end = "<<file_end<<endl;
		} else if (variable_name == "file_num_interval") {
			file_n_interval = stoul(line.substr(pos+1,line.length()));
			if(myid==0)
				cout<<"file_num_interval = "<<file_n_interval<<endl;
		} else if (variable_name == "proc_num") {
			proc_num = stoul(line.substr(pos+1,line.length()));
			if(myid==0)
				cout<<"proc_num = "<<proc_num<<endl;
		}else if (variable_name == "suffix") {
			//cout<<variable_name<<endl;
			//field_data = stoul(line.substr(pos+1,line.length()));
			//cout<<line.substr(pos+1,line.find_last_not_of(" "))<<endl;
			/*if (line.substr(pos+1,line.find_last_not_of(" ")) == "true")
				field_data = true;
			else if (line.substr(pos+1,line.find_last_not_of(" ")) == "false"){
				
				field_data = false;
			}
			else
				throw runtime_error("Incorret input of field_data = "+line.substr(pos+1,line.find_last_not_of(" ")));
				*/
			suffix = line.substr(pos+1,line.length());
			if(myid==0){
				//cout<<"false"<<endl;
				/*if (field_data)
					cout<<"field_data = true"<<endl;
				else 
					cout<<"field_data = false"<<endl;*/
				cout<<"suffix = "<<suffix<<endl;
			}
		}
		
		return;
	}



};

string Control_parameter::Local_filename(){
	
	string local_filename = "sol_tec_";
	unsigned time_step = file_start + file_n_interval * (read_time*numprocs + myid);

	if (time_step > file_end){
		return "over";
	} else {
		local_filename = local_filename + to_string(time_step);
	}

	/*if (field_data)
		local_filename = local_filename + "_field";
	local_filename = local_filename + ".dat";*/
	local_filename = local_filename + suffix;
	read_time = read_time + 1;
	return local_filename;
};