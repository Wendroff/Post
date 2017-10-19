#include "zone.h"

Zone::Zone(vector<string> variable_names)
{
	number_of_nodes = number_of_elements = i_index = j_index = 0;
	variables.push_back(vector<float>());
	for (int i = 0; i < variable_names.size(); i++)
		var_to_int.insert(pair<string, int>(variable_names[i], i));
}

void Zone::set_format(Zone::Format p_format)
{
	format = p_format;
}

Zone::Format Zone::get_format()
{
	return format;
}

void Zone::set_element_type(Zone::Element_Type p_element_type)
{
	element_type = p_element_type;
}

Zone::Element_Type Zone::get_element_type()
{
	return element_type;
}

void Zone::set_number_of_nodes(int p_number_of_nodes)
{
	number_of_nodes = p_number_of_nodes;
}

void Zone::set_number_of_elements(int p_number_of_elements)
{
	number_of_elements = p_number_of_elements;
}

int Zone::get_number_of_nodes()
{
	return number_of_nodes;
}

int Zone::get_number_of_elements()
{
	return number_of_elements;
}

void Zone::insert_variable_value(float variable_value)
{
	if (j_index < variables.size())
	{
		variables[i_index].push_back(variable_value);
		j_index++;
		return;
	}
	i_index++;
	variables.push_back(vector<float>());
	variables[i_index].push_back(variable_value);
	j_index = 1;
}

float Zone::get_value(int index, string key)
{
	j_index = var_to_int[key];
	return variables[index][j_index];
}
