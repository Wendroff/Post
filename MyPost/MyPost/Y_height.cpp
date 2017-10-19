#include<math.h>

#include"Y_height.h"

using namespace std;

Y_height::Y_height(){y = 0.0;};

Y_height::Y_height(double a){y = a;};

bool operator== (const Y_height a, const Y_height b){
	if (fabs(a.y - b.y)<1e-7)
		return true;
	else
		return false;
}

bool operator!= (const Y_height a, const Y_height b){
	if (a==b)
		return false;
	else
		return true;
}
bool operator<  (const Y_height a, const Y_height b){
	if ((b.y-a.y)>=1e-7)
		return true;
	else
		return false;
}

bool operator>  (const Y_height a, const Y_height b){
	if ((a.y-b.y)>=1e-7)
		return true;
	else
		return false;
}
ostream &operator<< (ostream &os, const Y_height& p){
	os<<p.y;
	return os;
}