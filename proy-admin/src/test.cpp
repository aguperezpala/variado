#include <iostream>
#include <string>

#include "note.h"
#include "task.h"
#include "function.h"
#include "module.h"
#include "parser.h"
#include "parser_aux.h"

int main(void)
{
	Module module;
	string fname = "./parse_test_file.h";
	
	cout << "parse_file(fname, Module &m): " << parse_file(fname,module);
	module.Print();
	
	return 0;
}

