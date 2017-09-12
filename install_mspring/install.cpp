/*
*  install.cpp
*  MSpring
*
*  Created by kimbom on 2017. 9. 13...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#include<iostream>
#include<string>
#include<urlmon.h>            //URLDownloadToFileA
#include<vector>
#include<Windows.h>
#pragma comment(lib,"urlmon.lib")
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#include<sstream>
#include"Unzip.h"
BOOL PathFolderExistsA(char* dir) {
	DWORD ftyp = GetFileAttributesA(dir);
	return (ftyp&FILE_ATTRIBUTE_DIRECTORY);
}
bool CopyFolder(std::string source_folder, std::string target_folder) {
	std::string new_sf = source_folder + "\\*";
	char sf[MAX_PATH + 1];
	char tf[MAX_PATH + 1];

	strcpy_s(sf, MAX_PATH, new_sf.c_str());
	strcpy_s(tf, MAX_PATH, target_folder.c_str());

	sf[strlen(sf) + 1] = 0;
	tf[strlen(tf) + 1] = 0;

	SHFILEOPSTRUCTA s = { 0 };
	s.wFunc = FO_COPY;
	s.pTo = tf;
	s.pFrom = sf;
	s.fFlags = FOF_SILENT | FOF_NOCONFIRMMKDIR | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NO_UI;
	int res = SHFileOperationA(&s);
	return res == 0;
}
void Fail(std::string str) {
	std::cerr << str << std::endl;
	system("pause");
	exit(EXIT_FAILURE);
}
int main() {
	char _temp[MAX_PATH];
	GetTempPathA(MAX_PATH - 1, _temp);
	std::string temp = _temp;
	
	std::string file = temp + "MSpring.zip";
	std::cout << "Downloading MSpring lastest version..." << std::endl;
	if (PathFileExistsA(file.c_str()) == TRUE) {
		std::cout << "Remove current version..." << std::endl;
		DeleteFileA("MSpring.zip");
	}
	HRESULT r = URLDownloadToFileA(nullptr, "https://github.com/springkim/MSpring_repo/archive/master.zip", file.c_str(), 0, 0);
	if (r != S_OK) {
		Fail("File download fail!!");
	}
	std::cout << "installing MSpring..." << std::endl;
	scvl::UnZip(file.c_str());
	std::string src = file.substr(0, file.find_last_of('.')) + "\\MSpring_repo-master";
	
	if (CopyFolder(src, ".\\") == FALSE) {
		Fail("Move file fail");
	}
	std::cout << "Successfully updated" << std::endl;
	return 0;
}