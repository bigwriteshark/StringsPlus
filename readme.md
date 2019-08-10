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