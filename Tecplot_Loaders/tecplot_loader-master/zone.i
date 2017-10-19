/*File: zone.i*/

%module zone
%{
#include "zone.h"
%}

using namespace std;

class Zone
{
	int number_of_nodes;
	int number_of_elements;
	int i_index;
	int j_index;
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
	void insert_variable_value(float variable_value);
	float get_value(int index, string key);
};
