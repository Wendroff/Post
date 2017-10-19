#ifndef Y_HEIGHT_H
#define Y_HEIGHT_H
#include<iostream>

using namespace std;
class Y_height{
public:
	Y_height();
	Y_height(double a);
	friend bool operator== (const Y_height a, const Y_height b);
	friend bool operator!= (const Y_height a, const Y_height b);
	friend bool operator<  (const Y_height a, const Y_height b);
	friend bool operator>  (const Y_height a, const Y_height b);
	friend ostream &operator<< (ostream &os, const Y_height& p);
	inline double gety() const {return y;}
private:
	double y;
};



#endif