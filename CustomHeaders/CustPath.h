#pragma once
#ifdef _WIN32 
	#include <Windows.h>
#endif
#include <string>
#include <cstring>

std::string loadPath(char* relativePath) {
	char fullpath[1000];
#ifdef _WIN32
	_fullpath(fullpath, relativePath, MAX_PATH);		// path to image for windows
#else
	strcpy(fullpath, relativePath); //TODO make sure that this works for mac
#endif
	std::string str(fullpath);		// convert to std::string
	return str;
}