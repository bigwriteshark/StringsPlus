锘縖English readme]
strings+ command scan binary file, print all ascii strings and all unicode strings. it can run in windows and linux, it can run in x86 and x64.

Format:

[strings+] [path] [-n [filename]] [-q] [-l [length]] [-h]

    [strings+]      command name, if windows that is strings+.exe, if linux that is strings+
    [path]          source files' directory, example: c:\mydir  or /home/mydir
    [-n [filename]] source files wildcard, example: -n *.exe
    [-q]            be quiet, no output message. example: -q
    [-l [length]]   filter string's max length, no less than 2. example: -l 3 
    [-h]            show help message.
    
Sample in windows:
    1. path:>strings+.exe [path]                scan all files in [path]
    2. path:>strings+.exe [path] -n [filename]  scan [filename] in [path]
    3. path:>strings+.exe [path] -n [*.*]       scan *.* in [path]
    4. path:>strings+.exe [path] -n [*.exe] -l 3  scan string that length <= 3 from *.exe in [path]

Sample in linux:
    1. $./strings+ [path]                scan all files in [path]
    2. $./strings+ [path] -n '[filename]'  scan [filename] in [path]
    3. $./strings+ [path] -n '[*.*]'       scan *.* in [path]
    4. $./strings+ [path] -n '[*.exe]' -l 3  scan string that length <= 3 from *.exe in [path]
Output:
    Output to a file named [path]/t.[filename].txt
    
    
【中文说明】
strings+ 程序可以扫描二进制文件，打印出所有的ascii码字符串，和所有的unicode字符串。它可以运行在windows平台，或者linux平台。

格式:
    [strings+] [path] [-n [filename]] [-q] [-l [length]] [-h]
    [strings+]      命令名，如果是windows则为strings+.exe，如果是linux平台则是strings+
    [path]          需要扫描的文件的所在目录，请注意是目录，而不是全路径名（不包含文件名），例如: c:\mydir  or /home/mydir
    [-n [filename]] -n选项：指定要扫描的文件的文件名（可以包含通配符*,?），例如: -n *.exe，或者-n my.bin
    [-q]            -q选项：指定静默运行，运行时不打印输出结果. 比如: -q
    [-l [length]]   -l选项：指定一个字符串长度过滤，扫描出来的字符串如果长度小于指定长度，则被忽略，这个长度必须大于等于2. 例如: -l 3
    [-h]            -h选项：显示帮助信息.
    
windows下的使用举例:
    1. path:>strings+.exe [path]                  扫描[path]目录下所有的文件
    2. path:>strings+.exe [path] -n [filename]    扫描[path]目录下匹配[filename]的文件
    3. path:>strings+.exe [path] -n [*.*]         扫描[path]目录下所有的文件
    4. path:>strings+.exe [path] -n [*.exe] -l 3  扫描[path]目录下*.exe且长度<=3的字符串
    

Sample in linux:
    1. $./strings+ [path]                	   扫描[path]目录下所有的文件
    2. $./strings+ [path] -n '[filename]'    扫描[path]目录下匹配[filename]的文件
    3. $./strings+ [path] -n '[*.*]'         扫描[path]目录下所有的文件
    4. $./strings+ [path] -n '[*.exe]' -l 3  扫描[path]目录下*.exe且长度<=3的字符串
    
输出结果:
    扫描出来的字符串，会保存到一个文本文件里，该输出文件保存在和源文件相同的目录下，输出文件的名称和源文件名称相匹配，输出文件的名称样式为：[path]/t.[源文件名].txt。
