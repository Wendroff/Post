#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include "mesh.h"

using namespace std;

string Mesh::get_title()
{
	return title;
}

void Mesh::set_title(string value)
{
	title = value;
}

Mesh::Mesh()
{
	number_of_zones = 0;
}

void Mesh::insert_zone()
{
	zones.push_back(new Zone(variables));
	number_of_zones++;
}

void Mesh::insert_variable_name(string variable_name)
{
	variables.push_back(variable_name);
}

Zone *Mesh::get_current_zone()
{
	return zones[number_of_zones - 1];
}

void Mesh::variable_section_is_over()
{
	insert_zone();
}

int Mesh::get_number_of_zones()
{
	return number_of_zones;
}

Zone *Mesh::get_zone(int index)
{
	if (index > zones.size())
		return NULL;
	return zones[index];
}
