#ifdef _WIN64
#error support only x86
#endif
#ifdef _DEBUG
#error support only release
#endif
/*
*  install.cpp
*  ispring
*
*  Created by kimbom on 2017. 9. 21...
*  Copyright 2017 kimbom. All rights reserved.
*
*/
#include<iostream>
#include<string>
#include<urlmon.h>            //URLDownloadToFileA
#include<vector>
#include<Windows.h>
#include<sstream>
#define NO_OPENCV
#include"ispring/All.h"

std::string GetNewVersion() {
	std::string url_git = "https://raw.githubusercontent.com/springkim/MSpring_repo/master/mspring/version.txt";
	std::string html = ispring::Web::GetHtml(url_git);
	return html;
	/*std::string tag_begin = "&lt;ispring-version&gt;";
	std::string tag_end = "&lt;/ispring-version&gt;";
	size_t pos_beg = html.find(tag_begin) + tag_begin.length();
	size_t pos_end = html.find(tag_end);
	std::string version = html.substr(pos_beg, pos_end - pos_beg);
	if (version.length() == 0) {
		version = "Can't load new version information";
	}
	return version;*/
}
std::string GetCurrentVersion() {
	std::ifstream fin;
	std::string version = "Not installed";
	if (ispring::File::DirectoryExist("mspring")) {
		std::ifstream fin;
		fin.open("mspring/version.txt");
		if (fin.is_open() == true) {
			version.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());
		}
	}
	return version;
}

void Fail(std::string str) {
	std::cerr << str << std::endl;
	system("pause");
	exit(EXIT_FAILURE);
}
void DrawMFC() {
#define __R std::cout << ispring::xout.light_red
#define __G std::cout << ispring::xout.light_green
#define __B std::cout << ispring::xout.light_blue;
	
	__R; printf("                     .:III===iiii=I;:~.                        "); puts("");
	__R; printf("                  .~;;;;IIII=====iiiii+++i=I;.                 "); puts("");
	__R; printf("               ..~~:::;;;;III======iiiiiiiiii;                 "); puts("");
	__R; printf("              ...~~~~:::;;;IIII=====iii===ii+;                 "); puts("");
	__R; printf("              ~~~~::::::;;;;;III========ii++o;                 "); puts("");
	__R; printf("              ~~~::::;;;;IIIII===II==iii+++oOI                 "); puts("");
	__R; printf("              ::;.~:;;;;II=====ii==ii++++oooOI                 "); puts("");
	__R; printf("              ::;    IIII=~:;=ii+ii++++oooooOI                 "); puts("");
	__R; printf("              ::I  .  =Ii.   `o++i++++oooooOOI                 "); puts("");
	__G; printf("        .~III;"); __R; printf("; I; ` :+; `..o + o++++oooooOO8I                 "); puts("");
	__G; printf("     .~;IIII=="); __R;printf(";;=  ;i  =  +` .ooo+++oooooOOO8=                 "); puts("");
	__G; printf("  .~::;;;;IIII"); __R;printf("I=i  ;+;   IO``.Ooo+ooooooOOOO8=                 "); puts("");
	__G; printf("..~~~:::;;;III"); __R;printf( "==ii=ii+:~:oo``.OoOooooooOOOO88I                 "); puts("");
	__G; printf("..~~~~~::;;;;I"); __R;printf( "iiiii+++ooo+o+i=OOOooooOOOOO888oI;~.             "); puts("");
	__G; printf("~~~~::::::;;;;"); __R; printf("iii+++++++ooooOOOOOoooOOOO88Oo"); __B;printf( "+++++++i=I;:~.     "); puts("");
	__G; printf("~~:::::;;;;IIII"); __R; printf("I==io++oooooOOOOOOOoOOOOo++"); __B;printf( "iiiii=====iiiiiiI    "); puts("");
	__G; printf("::::;;IIIIIII==="); __R; printf("iii++++oooOIi+o88OOOoii"); __B;printf( "==================i=    "); puts("");
	__G; printf(":::;`   ``~====ii+i+++oooo8."); __B;printf( "~~~~~;;;;IIIIIIII=========iii+i    "); puts("");
	__G; printf(":;;;.  =;:~iii+++o+++ooooO8"); __B;printf( ".~:::::::;;;;;IIIIIIIII==iii++oi    "); puts("");
	__G; printf(";;;I.  :;=i+++++oo+ooooOOO8"); __B; printf(".:::::;;;;IIIIII=======iii+++oo+    "); puts("");
	__G; printf(";IIi.     :++++ooo+oooOOOOE"); __B; printf("~:::;;;II;;I======i=iiii+++oooo+    "); puts("");
	__G; printf("===i.  o+o+++ooooOoooOOOO8E"); __B; printf("~::;;I:       =iii+iii++++ooooO+    "); puts("");
	__G; printf("==iiI  ++++oooooOOooOOOO88E"); __B; printf("~;;;;:  .=i+i:=++++i+++++oooooO+    "); puts("");
	__G; printf("iiii++++++ooooOOOOoOOOO888#"); __B; printf(":;;;I~  =iiii+++++++++++oooooOO+    "); puts("");
	__G; printf("i+++++oooooooOOOO8OOOO88E8i"); __B; printf(";I===i  .+++++++++o++++oooooOO8o    "); puts("");
	__G; printf("` `~:;Ii+O88888888OO8EO=~  "); __B; printf("i====ii~  ~;I;ioooo+++oooooOOO8o.   "); puts("");
	__G; printf("           ~:I=+OEEoI.     "); __B; printf("i==iiii+iI:~..iooooo+oooooOO88Eo.   "); puts("");
	__B; printf("                           +iiiiii+++ooooooooOoooooOO88oI      "); puts("");
	__B; printf("                           .~:;Ii+ooooooooooOOoooOO8+;         "); puts("");
	__B; printf("                                   .~;Ii+O8888OOOi:            "); puts("");
	__B; printf("                                           .~;I~               "); puts("");
	std::cout << ispring::xout.light_white;
}
void DrawInfo() {
	puts("=============================================");
	puts("= MSpring : MFC Design Library by springkim =");
	puts("=         Installer version : 2.0.0         =");
	puts("=          Build date : 2017-11-17          =");
	puts("=============================================");
}
int main() {
	system("color 0F");
	DrawMFC();
	DrawInfo();
	std::string curr_version = GetCurrentVersion();
	std::string new_version = GetNewVersion();

	std::cout << "Current version : " << curr_version << std::endl;
	std::cout << "New version : " << new_version << std::endl;
	if (curr_version != new_version) {
		char _temp[MAX_PATH];
		GetTempPathA(MAX_PATH - 1, _temp);
		std::string temp = _temp;

		std::string file = temp + "MSpring.zip";
		std::cout << "Downloading MSpring lastest version..." << std::endl;

		if (ispring::File::DirectoryExist(file) == true) {
			ispring::File::FileErase(file);
		}
		if (ispring::Web::Download("https://github.com/springkim/MSpring_repo/archive/master.zip", file) == false) {
			ISPRING_VERIFY("Can't download new version");
			exit(1);
		}
		std::cout << "installing MSpring..." << std::endl;
		ispring::Zip::Uncompress(file.c_str());
		ispring::File::DirectoryCopy(temp + "MSpring/MSpring_repo-master/mspring", "./mspring/");
		ispring::File::DirectoryErase(temp + "MSpring");
		ispring::File::FileErase(file);
		std::cout << "Successfully updated!!" << std::endl;
	} else {
		std::cout << "already installed!" << std::endl;
	}
	system("pause");
	return 0;
}