// string+.cpp  
//
#define _CRT_SECURE_NO_WARNINGS


#ifdef _WIN32
#include <io.h>
#include <string>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>

#define MAX_PATH 300

using namespace std;


//String's encode
enum enumEncode
{
	ASCII,
	Unicode,
};

struct struParam
{
	char full_path[MAX_PATH];
	char file_name[MAX_PATH];

	string tostring()
	{
		stringstream ss;
		ss << "struParam:" << endl;
		ss << "    full_path: " << full_path << endl;
		ss << "    file_name: " << file_name << endl;
		return ss.str();
	}
};
struParam g_Param;
const char* ParamFileName = "-name";
const char* ParamOption = "-o";


//Check and process argv
bool ProcessParams(int argc, const char* argv[], struParam& param);

//Get param's value by name
bool FindParameterValue(int argc, const char* argv[], const char* paramName, char* paramValue);

//Enum files from a folder
void EnumFilesFromFolder(string folderPath, int depth);

//Scan file and find unicode string or ascii string
bool GetStringsFromFile(const char * filename, enumEncode encode);

//Show help message
void ShowHelp();


//main
int main(int argc, const char* argv[])
{
	bool bret;

	ProcessParams(argc, argv, g_Param);
	cout << g_Param.tostring() << endl;
	EnumFilesFromFolder("./", 0);


	system("pause");
	return 0;
}


//Check and process argv
bool ProcessParams(int argc, const char* argv[], struParam& param)
{	
	//check param
	if (argc < 2)
	{
		ShowHelp();
		return false;
	}

	strcpy(param.full_path, argv[1]);

	char value[MAX_PATH];
	if (FindParameterValue(argc, argv, ParamFileName, value))
	{
		strcpy(param.file_name, value);
	}
	else
	{
		strcpy(param.file_name, "*");
	}

	return true;
}


//Enum files from a folder
void EnumFilesFromFolder(string folderPath, int depth)
{
#ifdef _WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		cerr << "can not match the folder path" << endl;
		exit(-1);
	}
	do {
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//这个语句很重要
			if ((depth - 1 > 0) && (strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				string newPath = folderPath + "\\" + FileInfo.name;
				EnumFilesFromFolder(newPath, depth - 1);
			}
		}
		else
		{
			string filename = (folderPath + "\\" + FileInfo.name);
			cout << folderPath << "\\" << FileInfo.name << " " << endl;
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if ((dp = opendir(folderPath.c_str())) == NULL) {
		fprintf(stderr, "cannot open directory: %s\n", folderPath.c_str());
		return;
	}
	chdir(folderPath.c_str());
	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {

			if (strcmp(".", entry->d_name) == 0 ||
				strcmp("..", entry->d_name) == 0)
				continue;
			printf("%*s%s/\n", depth, "", entry->d_name);
			if (depth - 1 > 0) {
				dfsFolder(entry->d_name, depth - 1);
			}
		}
		else {
			string filename = entry->d_name;
			printf("%*s%s\n", depth, "", entry->d_name);
		}
	}
	chdir("..");
	closedir(dp);
#endif
}


//Get param's value by name
bool FindParameterValue(int argc, const char* argv[], const char* paramName, char* paramValue)
{
	if (argv == NULL)
	{
		paramValue = NULL;
		return false;
	}
	for (int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], paramName) != 0 || i == argc - 1) continue;
		strcpy(paramValue, argv[i + 1]);
		return true;
	}

	paramValue = NULL;
	return false;
}


//Scan file and find unicode string or ascii string
bool GetStringsFromFile(const char * filename, enumEncode encode)
{
	return true;
}


//Show help message
void ShowHelp()
{
	printf("\nHelp:                                            ");
	printf("\n    1. $strings+ [path]                          ");
	printf("\n    1. $strings+ [path] -name [filename]         ");
	printf("\n    1. $strings+ [path] -name [*.*]              ");
	printf("\n    3. $strings+ [path] -name [*.*] -r opt       ");
	printf("\n");
}






 