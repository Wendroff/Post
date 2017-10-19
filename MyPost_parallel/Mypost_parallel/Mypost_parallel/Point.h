#ifndef POINT_H
#define POINT_H
#include<iostream>
using namespace std;

class Point{
public:
	Point();
	Point(double a, double b, double c);
	void print();
	inline double getx() const {return x;}
	inline double gety() const {return y;}
	inline double getz() const {return z;}
	friend bool operator== (const Point a, const Point b);
	friend bool operator!= (const Point a, const Point b);
	friend bool operator<  (const Point a, const Point b);
	friend bool operator>  (const Point a, const Point b);
	friend ostream &operator<< (ostream &os, const Point& p);
	void random_set(double start, double end); //随机生成一个点，所有坐标的范围在[start,end]
	bool is_x_same(const Point a);
	bool is_y_same(const Point a);
	bool is_z_same(const Point a);
private:
	double x,y,z; //点的坐标
};


//ostream & operator<<(ostream & os, Point & p);
#endif