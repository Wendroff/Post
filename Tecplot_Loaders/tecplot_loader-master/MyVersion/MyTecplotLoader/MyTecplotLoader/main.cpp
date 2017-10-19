//#include "tecplot_loader.tab.c"
#include "zone.h"
#include "mesh.h"


int main()
{
	//yydebug = 1;
	Mesh *mesh = parse("../meshes/title.dat");
	cout << mesh->get_title() << endl;
//	if (mesh->get_format() == Mesh::FEPOINT)
//		cout << "It's fepoint" << endl;
	return 0;
}
