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
#include <list>
#include <tuple>
#include <fstream>

#define MAX_PATH 300

using namespace std;


//String's encode
enum enumEncode
{
	ASCII,
	Unicode,
};


//struct of param, analysis argv
struct struParam
{
	char full_path[MAX_PATH];
	char file_name[MAX_PATH];
	bool is_quiet;
	int filter_length;

	struParam()
	{
		memset(full_path, 0, MAX_PATH);
		memset(file_name, 0, MAX_PATH);
		is_quiet = false;
		filter_length = 2;
	}

	string tostring()
	{
		stringstream ss;
		ss << "struParam:" << endl;
		ss << "    full_path: " << full_path << endl;
		ss << "    file_name: " << file_name << endl;
		return ss.str();
	}
};

const wchar_t* ll = L"lavender my love";
const char* ParamFileName = "-n";
const char* ParamQuiet = "-q";
const char* ParamFilterLength = "-l";
const char* ParamHelp = "-h";
struParam g_Param;
list<tuple<string, string, string>> g_lstFiles; //0.File path 1.File name 2.File full name


//Check and process argv
bool ProcessParams(int argc, const char* argv[], struParam& param);

//Get param's value by name
bool FindParameterValue(int argc, const char* argv[], const char* paramName, char* paramValue);

//Enum files from a folder
void EnumFilesFromFolder(string folderPath, string wildcard, int depth);

//Scan file and find unicode string or ascii string
bool GetStringsFromFile(tuple<string, string, string>& file);

//Show help message
void ShowHelp();


//main
int main(int argc, const char* argv[])
{
	bool bret;
	unsigned char ccc = 0xd6;
	int vvv = isprint(ccc);

	//Check and process params
	if (!ProcessParams(argc, argv, g_Param))
	{
		return 0;
	}

	//Get file list
	g_lstFiles.clear();
	EnumFilesFromFolder(g_Param.full_path, g_Param.file_name, 0);

	//Process each of the file
	for (auto f : g_lstFiles)
	{
		GetStringsFromFile(f);
	}

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

	memset(value, 0, MAX_PATH);
	if (FindParameterValue(argc, argv, ParamFileName, value))
	{
		strcpy(param.file_name, value);
	}

	memset(value, 0, MAX_PATH);
	if (FindParameterValue(argc, argv, ParamQuiet, value))
	{
		param.is_quiet = true;
	}

	memset(value, 0, MAX_PATH);
	if (FindParameterValue(argc, argv, ParamFilterLength, value))
	{
		int len = atoi(value);
		param.filter_length = (len < 2 ? len = 2 : len);
	}

	memset(value, 0, MAX_PATH);
	if (FindParameterValue(argc, argv, ParamHelp, value))
	{
		ShowHelp();
		return false;
	}

	return true;
}


//Enum files from a folder
void EnumFilesFromFolder(string folderPath, string wildcard, int depth)
{
#ifdef _WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\" + wildcard;
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
				EnumFilesFromFolder(newPath, wildcard, depth - 1);
			}
		}
		else
		{
			string filename = (folderPath + "\\" + FileInfo.name);
			g_lstFiles.push_back(make_tuple(folderPath, FileInfo.name, filename));
			//cout << folderPath << "\\" << FileInfo.name << " " << endl;
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
bool GetStringsFromFile(tuple<string, string, string>& file)
{

	string fname = std::get<2>(file);
	string oname = std::get<0>(file) + string("/t.") + std::get<1>(file) + string(".txt");
	ifstream io = ifstream(fname, std::ios::binary);
	ofstream oo = ofstream(oname);
	list<unsigned char> lstAStr;
	int pos = 0;


	auto ProcessString = [&]()
	{
		if (lstAStr.size() < g_Param.filter_length)
		{
			lstAStr.clear();
			return;
		}

		oo << "\t" << pos-lstAStr.size() << "\t\t";
		while (!lstAStr.empty())
		{
			unsigned char c = lstAStr.front();
			if(!g_Param.is_quiet) printf("%c", c);
			oo.put(c);
			lstAStr.pop_front();
		}
		oo << endl;
		if (!g_Param.is_quiet) printf("\n");
	};


	//ascii strings
	{
		unsigned char c;
		io.seekg(0, ios::beg);
		oo << "\nASCII string:" << endl;
		pos = 0;
		while (!io.eof())
		{
			c = io.get();
			pos++;

			if (isprint(c))
			{
				lstAStr.push_back(c);
			}
			else
			{
				ProcessString();
			}
		}
		oo << endl;
		oo << endl;
	}

	//unicode strings
	{
		unsigned char c;
		io.clear();
		io.seekg(0, ios::beg);
		oo << "\nUnicode string:" << endl;
		pos = 0;
		while (!io.eof())
		{
			c = io.get();
			pos++;
			if (isprint(c))
			{
				if (io.eof()) break;
				unsigned d = io.get();
				pos++;
				if (d == 0)
				{
					lstAStr.push_back(c);
					lstAStr.push_back(' ');
				}
				else
				{
					ProcessString();
				}
			}
			else
			{
				if (io.eof()) break;
				ProcessString();
			}
		}
		oo << endl;
		oo << endl;
	}

	io.close();
	oo.close();

	return true;
}


//Show help message
void ShowHelp()
{
	printf("\nFormat:                                                                                        ");
	printf("\n[strings+] [path] [-n [filename]] [-q] [-l [length]] [-h]                                      ");
	printf("\n    [strings+]      command name, if windows that is strings+.exe, if linux that is strings+   ");
	printf("\n    [path]          source files' directory, example: c:\mydir  or /home/mydir                 ");
	printf("\n    [-n [filename]] source files wildcard, example: -n *.exe                                   ");
	printf("\n    [-q]            be quiet, no output message. example: -q                                   ");
	printf("\n    [-l [length]]   filter string's max length, no less than 2. example: -l 3                  ");
	printf("\n    [-h]            show help message.                                                         ");
	printf("\nSample in windows:                                                                             ");
	printf("\n    1. path:>strings+.exe [path]                scan all files in [path]                       ");
	printf("\n    2. path:>strings+.exe [path] -n [filename]  scan [filename] in [path]                      ");
	printf("\n    3. path:>strings+.exe [path] -n [*.*]       scan *.* in [path]                             ");
	printf("\n    4. path:>strings+.exe [path] -n [*.exe] -l 3  scan string that length <= 3 from *.exe in [path]     ");
	printf("\nSample in linux:                                                                               ");
	printf("\n    1. $./strings+ [path]                scan all files in [path]                              ");
	printf("\n    2. $./strings+ [path] -n [filename]  scan [filename] in [path]                             ");
	printf("\n    3. $./strings+ [path] -n [*.*]       scan *.* in [path]                                    ");
	printf("\n    4. $./strings+ [path] -n [*.exe] -l 3  scan string that length <= 3 from *.exe in [path]   ");
	printf("\nOutput:                                                                                        ");
	printf("\n    Output to a file named [path]/t.[filename].txt                                             ");
	printf("\n");
}






 