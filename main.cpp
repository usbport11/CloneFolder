#include <windows.h>
#include <dir.h>
#include <iostream>
#include <vector>
#include <string>

std::string basePath;
std::string clonePath;

void FindFile(const std::string &directory) {
	std::string searchFilter = directory + "\\*";
	WIN32_FIND_DATA file;
	HANDLE searchHandle = FindFirstFile(searchFilter.c_str(), &file);
	if(searchHandle != INVALID_HANDLE_VALUE) {
		std::string currentValue;
		std::vector<std::string> directories;
		std::vector<std::string>::iterator it;
		while(FindNextFile(searchHandle, &file)) {
			if(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (!strcmp(file.cFileName, ".") || !strcmp(file.cFileName, ".."))) {
				continue;
			}
			searchFilter = directory + "\\" + std::string(file.cFileName);
			currentValue = clonePath; 
			currentValue += searchFilter.substr(basePath.length(), searchFilter.length() - basePath.length());
			if(file.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
				if(CreateFile(currentValue.c_str(), GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL) == INVALID_HANDLE_VALUE) {
					std::cout<<" Error: "<<GetLastError()<<std::endl;
				}
			}
			std::cout<<searchFilter<<" to "<<currentValue<<std::endl;
			
			std::cout<<searchFilter.c_str()<<" "<<currentValue<<std::endl;
			if(file.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
				directories.push_back(searchFilter);
				if(!CreateDirectory(currentValue.c_str(), NULL)) {
					std::cout<<" Error: "<<GetLastError()<<std::endl;
				}
			}
		}
		FindClose(searchHandle);
		for(it = directories.begin(); it != directories.end(); it++) {
			FindFile(*it);
		}
	}
}

int main(int argc, char** argv) {
	if(argc < 3) {
		std::cout<<"Usage:"<<std::endl;
		std::cout<<" CloneFolder <folder destination> <folder source>"<<std::endl;
		std::cout<<"Emample:"<<std::endl;
		std::cout<<" CloneFolder E:\\Clone E:\\FTP "<<std::endl;
		return 1;
	}
	basePath = argv[2];
	clonePath = argv[1];
	std::cout<<"Start clone from "<<argv[1]<<" to "<<argv[2]<<std::endl;
	FindFile(basePath);
	return 0;
}
