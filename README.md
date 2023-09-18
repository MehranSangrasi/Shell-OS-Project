
# C++ Shell Project
This is a C++ shell project that provides various file management and process management commands. It is developed as a student project and aims to provide a command-line interface for performing different operations on files and processes.

## Team
- Mehran Wahid
- Abdullah Saim - [Github](https://github.com/ASa1m)
- Hamza Amir - [Github](https://github.com/MHamzaAamir)
## Features
- Change directory (cd) 
- Print current working directory (pwd)
- Clear the screen (clear)
- Print environment variables (env)
- Run C++ program (runcpp <filename>)
- Find text in files (find <text> <directory>)
- Get file statistics (filestat <filename>)
- Receive files from a server (recieve)
- Send files to a server (send)
- Execute a command as root user (exroot <directory>)
- Download a file from a URL (download <url> <output file>)
- Delete empty directories (deleteEmpty <directory>)
- Organize files by extension (orgext <directory>)
- Print process information (ps <process id>)
- Swap contents of two files (swap <filename1> <filename2>)
- Rename files in a directory (prename <directory> <extension> <new name>)
## Usage
To use this shell, compile and run the command_line_arguments.cpp file.

bash
Copy code
g++ command_line_arguments.cpp -o shell
./shell
Once you start the shell, you can enter commands based on the available features. Here are some examples:

1. Change directory: **cd <directory>**
2. Print current working directory: **pwd**
3. Clear the screen: **clear**
4. Print environment variables: **env**
5. Run a C++ program: **runcpp <filename>**
6. Find text in files: find **<text> <directory>**
7. Get file statistics: **filestat <filename>**
8. Receive files from a server: **recieve**
9. Send files to a server: **send**
10. Execute a command as root user: **exroot <directory>**
11. Download a file from a URL: **download <url> <output file>**
12. Delete empty directories: **deleteEmpty <directory>**
13. Organize files by extension: **orgext <directory>**
14. Print process information: **ps <process id>**
15. Swap contents of two files: **swap <filename1> <filename2>**
16. Rename files in a directory: **prename <directory> <extension> <new name>**
 
## Dependencies
This project depends on the following libraries:
- <iostream.h>
- <string.h>
- <unistd.h>
- <cstdlib.h>
- <stdio.h>
- <stdlib.h>
- <time.h>
- <dirent.h>
- <ctime.h>
- <filesystem.h>
- <sys/stat.h>
- <fstream.h>
- <sstream.h>
- <algorithm.h>
- <sys/types.h>
- <sys/wait.h>
Make sure you have these libraries installed before compiling the project.

## Acknowledgement
This project is developed by students and may contain bugs or limitations. Use it at your own risk. For any issues or suggestions, please contact the developer.

Mehran Sangrasi
Abdullah Saim
Muhammad Hamza Amir
