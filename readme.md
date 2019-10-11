\uFEFF

[English readme]

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
    
    
    
\u3010\u4E2D\u6587\u8BF4\u660E\u3011

strings+ \u7A0B\u5E8F\u53EF\u4EE5\u626B\u63CF\u4E8C\u8FDB\u5236\u6587\u4EF6\uFF0C\u6253\u5370\u51FA\u6240\u6709\u7684ascii\u7801\u5B57\u7B26\u4E32\uFF0C\u548C\u6240\u6709\u7684unicode\u5B57\u7B26\u4E32\u3002\u5B83\u53EF\u4EE5\u8FD0\u884C\u5728windows\u5E73\u53F0\uFF0C\u6216\u8005linux\u5E73\u53F0\u3002

\u683C\u5F0F:

[strings+] [path] [-n [filename]] [-q] [-l [length]] [-h]
    [strings+]      \u547D\u4EE4\u540D\uFF0C\u5982\u679C\u662Fwindows\u5219\u4E3Astrings+.exe\uFF0C\u5982\u679C\u662Flinux\u5E73\u53F0\u5219\u662Fstrings+
    [path]          \u9700\u8981\u626B\u63CF\u7684\u6587\u4EF6\u7684\u6240\u5728\u76EE\u5F55\uFF0C\u8BF7\u6CE8\u610F\u662F\u76EE\u5F55\uFF0C\u800C\u4E0D\u662F\u5168\u8DEF\u5F84\u540D\uFF08\u4E0D\u5305\u542B\u6587\u4EF6\u540D\uFF09\uFF0C\u4F8B\u5982: c:\mydir  or /home/mydir
    [-n [filename]] -n\u9009\u9879\uFF1A\u6307\u5B9A\u8981\u626B\u63CF\u7684\u6587\u4EF6\u7684\u6587\u4EF6\u540D\uFF08\u53EF\u4EE5\u5305\u542B\u901A\u914D\u7B26*,?\uFF09\uFF0C\u4F8B\u5982: -n *.exe\uFF0C\u6216\u8005-n my.bin
    [-q]            -q\u9009\u9879\uFF1A\u6307\u5B9A\u9759\u9ED8\u8FD0\u884C\uFF0C\u8FD0\u884C\u65F6\u4E0D\u6253\u5370\u8F93\u51FA\u7ED3\u679C. \u6BD4\u5982: -q
    [-l [length]]   -l\u9009\u9879\uFF1A\u6307\u5B9A\u4E00\u4E2A\u5B57\u7B26\u4E32\u957F\u5EA6\u8FC7\u6EE4\uFF0C\u626B\u63CF\u51FA\u6765\u7684\u5B57\u7B26\u4E32\u5982\u679C\u957F\u5EA6\u5C0F\u4E8E\u6307\u5B9A\u957F\u5EA6\uFF0C\u5219\u88AB\u5FFD\u7565\uFF0C\u8FD9\u4E2A\u957F\u5EA6\u5FC5\u987B\u5927\u4E8E\u7B49\u4E8E2. \u4F8B\u5982: -l 3
    [-h]            -h\u9009\u9879\uFF1A\u663E\u793A\u5E2E\u52A9\u4FE1\u606F.
    
    
windows\u4E0B\u7684\u4F7F\u7528\u4E3E\u4F8B:
    1. path:>strings+.exe [path]                  \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u6240\u6709\u7684\u6587\u4EF6
    2. path:>strings+.exe [path] -n [filename]    \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u5339\u914D[filename]\u7684\u6587\u4EF6
    3. path:>strings+.exe [path] -n [*.*]         \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u6240\u6709\u7684\u6587\u4EF6
    4. path:>strings+.exe [path] -n [*.exe] -l 3  \u626B\u63CF[path]\u76EE\u5F55\u4E0B*.exe\u4E14\u957F\u5EA6<=3\u7684\u5B57\u7B26\u4E32
    

Sample in linux:

    1. $./strings+ [path]                	   \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u6240\u6709\u7684\u6587\u4EF6
    2. $./strings+ [path] -n '[filename]'    \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u5339\u914D[filename]\u7684\u6587\u4EF6
    3. $./strings+ [path] -n '[*.*]'         \u626B\u63CF[path]\u76EE\u5F55\u4E0B\u6240\u6709\u7684\u6587\u4EF6
    4. $./strings+ [path] -n '[*.exe]' -l 3  \u626B\u63CF[path]\u76EE\u5F55\u4E0B*.exe\u4E14\u957F\u5EA6<=3\u7684\u5B57\u7B26\u4E32
    
    
\u8F93\u51FA\u7ED3\u679C:

    \u626B\u63CF\u51FA\u6765\u7684\u5B57\u7B26\u4E32\uFF0C\u4F1A\u4FDD\u5B58\u5230\u4E00\u4E2A\u6587\u672C\u6587\u4EF6\u91CC\uFF0C\u8BE5\u8F93\u51FA\u6587\u4EF6\u4FDD\u5B58\u5728\u548C\u6E90\u6587\u4EF6\u76F8\u540C\u7684\u76EE\u5F55\u4E0B\uFF0C\u8F93\u51FA\u6587\u4EF6\u7684\u540D\u79F0\u548C\u6E90\u6587\u4EF6\u540D\u79F0\u76F8\u5339\u914D\uFF0C\u8F93\u51FA\u6587\u4EF6\u7684\u540D\u79F0\u6837\u5F0F\u4E3A\uFF1A[path]/t.[\u6E90\u6587\u4EF6\u540D].txt\u3002