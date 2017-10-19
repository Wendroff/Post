#ifndef ZONE_H
#define ZONE_H

#include <vector>
#include <map>
#include <string>
using namespace std;

class Zone
{
	int number_of_nodes;
	int number_of_elements;
	unsigned i_index;
	unsigned j_index;
	vector < vector<float> > variables;
	map    < string, int > var_to_int;
public:
	Zone(vector<string> variable_names);

	enum Format
	{
		POINT,
		FEPOINT,
		BLOCK,
		FEBLOCK
	};
	Format format;

	enum Element_Type
	{
		TRIANGLE,
		BRICK,
		QUADRILATERAL,
		TETRAHEDRON
	};
	Element_Type element_type;

	void set_format(Zone::Format p_format);
	Zone::Format get_format();
	void set_element_type(Zone::Element_Type p_element_type);
	Zone::Element_Type get_element_type();
	int get_number_of_nodes();
	int get_number_of_elements();
	void set_number_of_nodes(int value);
	void set_number_of_elements(int value);
	// we got two indices: i_index and j_index. i serves as the first dimension index for the variables vector while j serves as the 2nd. we keep pushing all the variable values contained in one line into one vector until it's over then we start pushing into the next one until the file is over.
	void insert_variable_value(float variable_value);
	float get_value(int index, string key);
};

#endif // ZONE_H
