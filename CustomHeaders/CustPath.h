#pragma once
#ifdef _WIN32 
	#include <Windows.h>
#endif
#include <string>
#include <cstring>

std::string loadPath(char* relativePath) {
	char fullpath[1000];
#ifdef _WIN32
	GetFullPathName(relativePath, MAX_PATH, fullpath, NULL);		// path to image for windows
#else
	strcpy(fullpath, relativePath); //path to image for mac
#endif
	std::string str(fullpath);		// convert to std::string
	return fullpath;
}