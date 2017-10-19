#include<iostream>
#include<ctime>
#include<cstdlib>
#include<math.h>
#include"Point.h"
using namespace std;

Point::Point(){x=0.;y=0.;z=0.;}

Point::Point(double a, double b, double c):x(a), y(b), z(c){};

void Point::print(){
	cout<<"( "<<x<<", "<<y<<", "<<z<<" )";
}

ostream &operator<< (ostream &os, const Point& p){
	os<<"( "<<p.x<<", "<<p.y<<", "<<p.z<<" )";
	return os;
}

bool operator== (const Point a, const Point b){
	if((fabs(a.x-b.x)<1e-7)&&(fabs(a.y-b.y)<1e-7)&&(fabs(a.z-b.z)<1e-7))
		return true;
	else
		return false;
}

bool operator!= (const Point a, const Point b){
	if(a==b)
		return false;
	else
		return true;
}



bool operator> (const Point a, const Point b){
	if ((a.x - b.x)>=1e-7) {
		return true;
	} else if (fabs(a.x-b.x)<1e-7){
		if ((a.y - b.y)>=1e-7) {
			return true;
		} else if (fabs(a.y-b.y)<1e-7) {
			if ((a.z - b.z)>=1e-7) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool operator< (const Point a, const Point b){
	
	if ((b.x - a.x)>=1e-7) {
		return true;
	} else if (fabs(a.x-b.x)<1e-7){
		if ((b.y - a.y)>=1e-7) {
			return true;
		} else if (fabs(a.y-b.y)<1e-7) {
			if ((b.z - a.z)>=1e-7) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Point::random_set(double start, double end){
	srand(unsigned (time(NULL)));
	x = start+(end-start)*rand()/(RAND_MAX + 1.0);
	y = start+(end-start)*rand()/(RAND_MAX + 1.0);
	z = start+(end-start)*rand()/(RAND_MAX + 1.0);
}

bool Point::is_x_same(const Point a){
	if(fabs(x-a.getx())<1e-7)
		return true;
	else
		return false;
};

bool Point::is_y_same(const Point a){
	if(fabs(y-a.gety())<1e-7)
		return true;
	else
		return false;
};

bool Point::is_z_same(const Point a){
	if(fabs(z-a.getz())<1e-7)
		return true;
	else
		return false;
};
