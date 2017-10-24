#include<fstream>
#include<string>
#include<iostream>
#include<stdlib.h>
#include<sstream>

using namespace std;
int main(int argc,char *argv[]){
	ifstream  input;
	ofstream  output("ForceX.dat");
	string    a;
	//istringstream time; //用于从字符串“t=****”读取数字
	//int       i=0;
	double    Force;
	double    t,T;
	int       iter;
	double    dt;

	//cout.precision(12);
	output.precision(12);
	//time.precision(12);
	output<<"Variables = t iter dt Time ForceX"<<endl;
	for (int i=1; i<argc; i++){
		input.open(argv[i]);
		while(input>>a){
			//cout<<a;
		

			if (a=="ForceX"){
				input>>a;
				input>>Force;
				while(input>>a){
					if(a.compare(0,5,"iter=") == 0){
						a.erase(0,5);
						iter=stoi(a);
						
						input>>a;
						if (a.compare(0,2,"t=") != 0){
							throw runtime_error("iter= is not followed by a t= ");
						}
						a.erase(0,2);
						t=stof(a);

						input>>a;
						if (a.compare(0,3,"dt=") != 0){
							throw runtime_error("t= is not followed by a dt= ");
						}
						a.erase(0,3);
						dt= stof(a);
						T = dt * double(iter);
						break;
					}
				}
				output<<t    <<"  "
					  <<iter <<" "
					  <<dt   <<" "
					  <<T    <<" "
					  <<Force<<endl;
				//cout  <<t<<"  "<<Force<<endl;
				//i++=;
	//			cout<<(++i)<<endl;
			}
			/*if (a=="ForceY"){
				input>>a;
				input>>Force[1];
				i++=;
			}
			if (a=="ForceZ"){
				input>>a;
				input>>Force[2];
				i++=;
			}*/
		
		}
		//cin>>i;
		//system("pause");
		input.close();
	}
	output.close();
	cout<<"The ForceX date has been written into ForceX.dat"<<endl;
	return 0;
}