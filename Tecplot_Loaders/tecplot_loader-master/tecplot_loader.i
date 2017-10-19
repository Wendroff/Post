/* tecplot_loader.i */

%module tecplot_loader
%{
	#include "tecplot_loader.tab.c"	
%}
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>
%include <std_pair.i>
%include mesh.i
%include zone.i

extern Mesh* parse(std::string path);
