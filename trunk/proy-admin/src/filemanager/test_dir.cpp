#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "dirmanager.h"



int main(void)
{
	vector<string> files;
	vector<string>::iterator it;
	string dir = ".";
	
	
	
	cout <<"returned value: " << dirm_list_files(dir, files, NULL) << endl;
	for (it = files.begin(); it != files.end(); ++it) 
		cout << "file: " << *it<< endl;
	cout << "\n\nBORRANDO\n" << endl;
	drim_filter(files, "dir", NULL);
	for (it = files.begin(); it != files.end(); ++it) 
		cout << "file: " << *it<< endl;
	
	files.clear();
	
	
	
	return 0;
}