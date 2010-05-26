#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "dir_mod_aux.h"



int main(void)
{
	vector<string> files;
	vector<string>::iterator it;
	string dir = "/home/agustin/programacion/video/variado/";
	//string dir = ".";
	
	
	
	/*cout <<"returned value: " << dirm_list_files(dir, files, NULL) << endl;
	for (it = files.begin(); it != files.end(); ++it) 
		cout << "file: " << *it<< endl;
	cout << "\n\nBORRANDO\n" << endl;
	dirm_filter(files, "dir", NULL);
	for (it = files.begin(); it != files.end(); ++it) 
		cout << "file: " << *it<< endl;
	
	
	files.clear();
	*/
	dirm_get_rdirs(files, dir);
	//dirm_list_directories(dir, files, NULL);
	//dirm_filter(files, ".", NULL);
	for (it = files.begin(); it != files.end(); ++it) 
		cout << "file: " << *it<< endl;
	
	
	files.clear();
	
	
	return 0;
}