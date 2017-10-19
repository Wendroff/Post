%{
#include <cstdio>
#include <iostream>
#include <string>
#include "mesh.h"
using namespace std;

#define YYDEBUG 1

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

Mesh *mesh =  new Mesh();

void yyerror(const char *s);
%}

%union {
	char *sval;
	float fval;
	int ival;
}
%define parse.error verbose
%define parse.trace true

%token TITLE
%token VARIABLES
%token ZONE
%token COMA
%token EQUALS

%token <sval> STRING
%token <fval> FLOAT
%token <ival> INTEGER

%token N
%token E
%token F
%token ET
%token POINT
%token FEPOINT
%token BLOCK
%token FEBLOCK
%token TRIANGLE
%token BRICK
%token QUADRILATERAL
%token TETRAHEDRON

%%

file:
	header
	;
header:
	title_section variables_section zone_sections
	;
title_section:
	TITLE EQUALS STRING { mesh->set_title($3); }
	;
variables_section:
	VARIABLES EQUALS variable_names { mesh->variable_section_is_over(); }
	;
variable_names:
	variable_names variable_name
	| variable_name
	;
variable_name:
	STRING COMA	{ mesh->insert_variable_name($1); }
	| STRING	{ mesh->insert_variable_name($1); }
	;
zone_sections:
	zone_sections zone_section { mesh->insert_zone(); }
	| zone_section
	;
zone_section:
	ZONE zone_attributes zone_body
	;
zone_attributes:
	zone_attributes zone_attribute
	| zone_attribute
	;
zone_attribute:
	attribute COMA
	| attribute
	;
attribute:
	N EQUALS INTEGER	{ mesh->get_current_zone()->set_number_of_nodes($3); }
	| E EQUALS INTEGER	{ mesh->get_current_zone()->set_number_of_elements($3); }
	| element_type
	| format
	;
element_type:
	ET EQUALS TRIANGLE	{ mesh->get_current_zone()->set_element_type(Zone::TRIANGLE); }
	| ET EQUALS BRICK	{ mesh->get_current_zone()->set_element_type(Zone::BRICK); }
	| ET EQUALS QUADRILATERAL { mesh->get_current_zone()->set_element_type(Zone::QUADRILATERAL); }
	| ET EQUALS TETRAHEDRON { mesh->get_current_zone()->set_element_type(Zone::TETRAHEDRON); }
	;
format:
	F EQUALS POINT		{ mesh->get_current_zone()->set_format(Zone::POINT); }
	| F EQUALS FEPOINT	{ mesh->get_current_zone()->set_format(Zone::FEPOINT); }
	| F EQUALS BLOCK	{ mesh->get_current_zone()->set_format(Zone::BLOCK); }
	| F EQUALS FEBLOCK	{ mesh->get_current_zone()->set_format(Zone::FEBLOCK); }
	;
zone_body:
	body_lines
	;
body_lines:
	body_lines body_line
	| body_line
	;
body_line:
	variable_values
	;
variable_values:
	variable_values variable_value
	| variable_value
	;
variable_value:
	FLOAT		{ mesh->get_current_zone()->insert_variable_value($1); }
	| INTEGER	{ mesh->get_current_zone()->insert_variable_value($1); }
	;
%%



Mesh* parse(std::string path)
{
	FILE *myfile = fopen(path.c_str(), "r");
	if (!myfile) {
		cout << "I can't open the mesh!" << endl;
		return mesh;
	}
	yyin = myfile;
	
	do {
		yyparse();
	} while (!feof(yyin));
	
	return mesh;
}

void yyerror(const char *error_message) {
	cout << error_message << endl;
	exit(-1);
}
