/*File: mesh.i*/

%module mesh
%{
#include "mesh.h"
%}

using namespace std;

class Mesh
{
	string title;
	int number_of_zones;
	vector < string > variables;
	vector < Zone* > zones;
public:

	Mesh();

	void insert_variable_name(string variable_name);
	string get_title();
	void set_title(string value);
	void insert_zone();
	Zone* get_current_zone();
	void variable_section_is_over();
	int get_number_of_zones();
	Zone* get_zone(int index);
};
