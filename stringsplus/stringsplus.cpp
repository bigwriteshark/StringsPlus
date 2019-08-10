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
#include <stack>
#endif

#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sstream>
#include <list>
#include <tuple>

#define MAX_PATH 300
#define MATCH_CHAR(c1,c2,ignore_case)  ( (c1==c2) || ((ignore_case) &&(tolower(c1)==tolower(c2))) )

using namespace std;


//String's encode
enum enumEncode
{
	ASCII,
	Unicode,
};


enum EnumFileType
{
	EnumFileType_File,
	EnumFileType_Directory
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
void EnumFilesFromFolder(string folderPath, string wildcard, int depth, list<tuple<string, string, string>>& lstFiles, bool inc_sub_dirs = false);

bool EnumWildCharMatch(const char *src, const char *pattern, bool ignore_case);

//Scan file and find unicode string or ascii string
bool GetStringsFromFile(tuple<string, string, string>& file, struParam param);

//Show help message
void ShowHelp();


//main
int main(int argc, const char* argv[])
{
	//Check and process params
	if (!ProcessParams(argc, argv, g_Param))
	{
		return 0;
	}

	//Get file list
	g_lstFiles.clear();
	EnumFilesFromFolder(g_Param.full_path, g_Param.file_name, 0, g_lstFiles);

	//Process each of the file
	for (auto f : g_lstFiles)
	{
		GetStringsFromFile(f, g_Param);
	}

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
void EnumFilesFromFolder(string folderPath, string wildcard, int depth, list<tuple<string, string, string>>& lstFiles, bool inc_sub_dirs)
{
	lstFiles.clear();

#ifdef _WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\" + wildcard;
	long Handle = (long)_findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		cerr << "can not match the folder path" << endl;
		exit(-1);
	}
	do {
		if (FileInfo.attrib & _A_SUBDIR)
		{
			if ((depth - 1 > 0) && (strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				string newPath = folderPath + "\\" + FileInfo.name;
				EnumFilesFromFolder(newPath, wildcard, depth - 1, lstFiles);
			}
		}
		else
		{
			string filename = (folderPath + "\\" + FileInfo.name);
			lstFiles.push_back(make_tuple(folderPath, FileInfo.name, filename));
			//cout << folderPath << "\\" << FileInfo.name << " " << endl;
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	EnumFileType type = EnumFileType_File;
	unsigned int nFilePerDir = 0;

	char real_path[260];
	size_t length = folderPath.length();

	strcpy(real_path, folderPath.c_str());
	if (real_path[length - 1] != '/')
		strcat(real_path, "/");

	stack<string> ps;
	ps.push(real_path);

	while (!ps.empty())
	{
		unsigned int nAlreadyCount = 0;
		string search_path = ps.top();
		ps.pop();

		DIR* dir = opendir((search_path).c_str());

		if (dir != 0)
		{
			struct stat file_stat;
			struct dirent *s_dir = readdir(dir);

			do
			{
				if ((strcmp(s_dir->d_name, ".") == 0) || (strcmp(s_dir->d_name, "..") == 0)) continue;

				stat((search_path + s_dir->d_name).c_str(), &file_stat);

				bool curIsDir = S_ISDIR(file_stat.st_mode);
				if ((type == EnumFileType_File && !curIsDir) ||
					(type == EnumFileType_Directory && curIsDir))
				{
					if (EnumWildCharMatch(s_dir->d_name, wildcard.c_str(), false))
					{
						string filename = (search_path + s_dir->d_name);
						lstFiles.push_back(make_tuple(search_path, s_dir->d_name, filename));
					}

					if (inc_sub_dirs)
						ps.push(search_path + s_dir->d_name + "/");

					if (nFilePerDir > 0 && ++nAlreadyCount == nFilePerDir) break;
				}
				else
				{
					if (inc_sub_dirs && curIsDir)
						ps.push(search_path + s_dir->d_name + "/");
				}
			} while ((s_dir = readdir(dir)) != 0);
			closedir(dir);
		}
	}
#endif
}



bool EnumWildCharMatch(const char *src, const char *pattern, bool ignore_case)
{
	bool result = false;

	while (*src)
	{
		if (*pattern == '*')
		{
			while ((*pattern == '*') || (*pattern == '?'))
				pattern++;

			if (!*pattern) return true;

			while (*src && (!MATCH_CHAR(*src, *pattern, ignore_case)))
				src++;

			if (!*src) return false;

			result = EnumWildCharMatch(src, pattern, ignore_case);
			while ((!result) && (*(src + 1)) && MATCH_CHAR(*(src + 1), *pattern, ignore_case))
				result = EnumWildCharMatch(++src, pattern, ignore_case);

			return result;
		}
		else
		{
			if (MATCH_CHAR(*src, *pattern, ignore_case) || ('?' == *pattern))
				return EnumWildCharMatch(++src, ++pattern, ignore_case);
			else
				return false;
		}
	}

	if (*pattern)
		return (*pattern == '*') && (*(pattern + 1) == 0);

	return true;
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
		if (strcmp(argv[i], paramName) != 0 || i == argc - 1) continue;
		strcpy(paramValue, argv[i + 1]);
		return true;
	}

	paramValue = NULL;
	return false;
}


//Scan file and find unicode string or ascii string
bool GetStringsFromFile(tuple<string, string, string>& file, struParam param)
{

	string fname = std::get<2>(file);
	string oname = std::get<0>(file) + string("/t.") + std::get<1>(file) + string(".txt");
	ifstream io;
	io.open(fname, std::ios::binary);
	ofstream oo;
	oo.open(oname);
	list<unsigned char> lstAStr;
	int pos = 0;


	auto ProcessString = [&]()
	{
		if (lstAStr.size() < param.filter_length)
		{
			lstAStr.clear();
			return;
		}

		oo << "\t" << pos - lstAStr.size() << "\t\t";
		while (!lstAStr.empty())
		{
			unsigned char c = lstAStr.front();
			if (!param.is_quiet) printf("%c", c);
			oo.put(c);
			lstAStr.pop_front();
		}
		oo << endl;
		if (!param.is_quiet) printf("\n");
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
	printf("\n    [path]          source files' directory, example: c:\\mydir  or /home/mydir                 ");
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
	printf("\n    2. $./strings+ [path] -n '[filename]'  scan [filename] in [path]                             ");
	printf("\n    3. $./strings+ [path] -n '[*.*]'       scan *.* in [path]                                    ");
	printf("\n    4. $./strings+ [path] -n '[*.exe]' -l 3  scan string that length <= 3 from *.exe in [path]   ");
	printf("\nOutput:                                                                                        ");
	printf("\n    Output to a file named [path]/t.[filename].txt                                             ");
	printf("\n");
}






