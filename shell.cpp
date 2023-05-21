// command_line_arguments.cpp  

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <ctime>
#include <filesystem>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/wait.h>


#define TOKENSIZE 100
namespace fs = std::filesystem;

using namespace std;
void  StrTokenizer(char *line, char **argv);
void  myExecvp(char **argv);
int GetEnv();



void  myExecvp(char **argv)
{
	pid_t  pid;
	int    status;
	int childStatus;
	pid = fork();
	if(pid == 0)
	{
		childStatus = execvp(*argv, argv);
		if (childStatus < 0){
			cout<<"ERROR:wrong input"<<endl;
		}
		exit(0);

	}
	else if(pid < 0)
	{
		cout<< "somthing went wrong!"<<endl;
	}
	else 

	{   int status;
		waitpid(pid, &status , 0);

	}
}

void StrTokenizer(char *input, char **argv)
{
	char *stringTokenized;
	stringTokenized = strtok(input, " ");
	while(stringTokenized != NULL)
	{
		*argv++  = stringTokenized;
		stringTokenized = strtok(NULL, " ");
	}

	*argv = NULL;
}

int GetEnv()
{
	char *path2;
	char *arr2[250];
	char *Tokenized ;
	path2 = getenv("PATH");
	Tokenized = strtok(path2, ":");
	int k = 0;
	while(Tokenized != NULL)
	{
		arr2[k] = Tokenized;
		Tokenized = strtok(NULL, ":");
		k++;
	}

	arr2[k] = NULL;
}

void ClearScreen()
{
	cout << string( 100, '\n' );
}

void ChangeDirectory(char *argv[])
{
	if (argv[1] == NULL)
	{
		cout << "ERROR: No directory specified" << endl;
	}
	else if(argv[1]=="..")
	{
		chdir("..");
	}
	else
	{
		if(chdir(argv[1]) != 0)
		{
			cout << "ERROR: Could not change directory" << endl;
		}
	}

}

void PrintWorkingDirectory()
{
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		cout << "Current working dir: " << cwd << endl;
	}
	else
	{
		cout << "ERROR: Could not get current working directory" << endl;
	}
}

void PrintEnvironment()
{
	extern char **environ;
	int i = 0;
	while (environ[i])
	{
		cout << environ[i++] << endl;
	}
}


void filestat(const std::string& filePath) {
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == 0) {
        std::cout << "File: " << filePath << std::endl;
        std::cout << "Size: " << fileStat.st_size << " bytes" << std::endl;
        std::cout << "Permissions: ";
        std::cout << ((fileStat.st_mode & S_IRUSR) ? "r" : "-");
        std::cout << ((fileStat.st_mode & S_IWUSR) ? "w" : "-");
        std::cout << ((fileStat.st_mode & S_IXUSR) ? "x" : "-");
        std::cout << ((fileStat.st_mode & S_IRGRP) ? "r" : "-");
        std::cout << ((fileStat.st_mode & S_IWGRP) ? "w" : "-");
        std::cout << ((fileStat.st_mode & S_IXGRP) ? "x" : "-");
        std::cout << ((fileStat.st_mode & S_IROTH) ? "r" : "-");
        std::cout << ((fileStat.st_mode & S_IWOTH) ? "w" : "-");
        std::cout << ((fileStat.st_mode & S_IXOTH) ? "x" : "-");
        std::cout << std::endl;
        std::cout << "Last accessed: " << std::ctime(&fileStat.st_atime);
        std::cout << "Last modified: " << std::ctime(&fileStat.st_mtime);
        std::cout << "Last status change: " << std::ctime(&fileStat.st_ctime);
    } else {
        std::cerr << "Failed to retrieve file information." << std::endl;
    }
}

void PrintHelp()
{
	cout << "cwushell is a simple shell program that can execute commands" << endl;
	cout << "The following commands are supported:" << endl;
	cout << "exit" << endl;
	cout << "cd" << endl;
	cout << "pwd" << endl;
	cout << "env" << endl;
	cout << "help" << endl;
	cout << "clear" << endl;
	cout << "find" << endl;
	cout << "runcpp" << endl;
	cout << "filestat" << endl;
	cout << "recieve" << endl;
	cout << "send" << endl;
	cout << "exroot" << endl;
}

void FindTextInDirectory(char *argv[])
{
    const char *searchText = argv[1];
    const char *directoryPath = argv[2];

    DIR *dir;
    struct dirent *entry;

    // Open the directory
    dir = opendir(directoryPath);
    if (dir == NULL)
    {
        std::cout << "Failed to open directory: " << directoryPath << std::endl;
        return;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Construct the full path of the entry
        std::string fullPath = directoryPath;
        fullPath += "/";
        fullPath += entry->d_name;

        // Get the file information
        struct stat fileInfo;
        if (stat(fullPath.c_str(), &fileInfo) == -1)
        {
            std::cout << "Failed to get file information for: " << fullPath << std::endl;
            continue;
        }

        // Check if the entry is a regular file
        if (S_ISREG(fileInfo.st_mode))
        {
            // Open the file
            FILE *file = fopen(fullPath.c_str(), "r");
            if (file == NULL)
            {
                std::cout << "Failed to open file: " << fullPath << std::endl;
                continue;
            }

            // Read the file line by line
            char line[256];
            int lineNumber = 1;
            while (fgets(line, sizeof(line), file))
            {
                // Check if the line contains the search text
                if (strstr(line, searchText) != NULL)
                {
                    std::cout << fullPath << ":" << lineNumber << ": " << line;
                }

                lineNumber++;
            }

            // Close the file
            fclose(file);
        }
    }

    // Close the directory
    closedir(dir);
}
void transferFilesToRootDirectory(const std::string& rootDirectory)
{
    for (const auto& entry : fs::recursive_directory_iterator(rootDirectory))
    {
        if (entry.is_regular_file())
        {
            const fs::path& filePath = entry.path();
            const fs::path& newFilePath = fs::path(rootDirectory) / filePath.filename();
            fs::rename(filePath, newFilePath);
            std::cout << "Transferred: " << filePath << " to " << newFilePath << std::endl;
        }
    }
}





void recieve()
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		execl("./server", "./server", NULL);
	}else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
	}
	else
	{
		perror("Fork failed.");
	}
}

void send()
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		execl("./client", "./client", NULL);
	}else if (pid > 0)
	{
		waitpid(pid, NULL, 0);
	}
	else
	{
		perror("Fork failed.");
	}
}

void RunCpp(const std::string& filename)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        execlp("g++", "g++", filename.c_str(), "-o", "main", NULL);
        exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);

		pid = fork();
		if (pid == 0)
		{
			
			execl("./main", "./main", NULL);
			exit(127);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			pid = fork();

			if (pid == 0)
			{
				execl("/bin/rm","rm","main",NULL);
				exit(127);
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
			}
			else
			{
				perror("Fork failed.");
			}
	
		}
		else
		{
			perror("Fork failed.");
		}
        
    }
    else
    {
        perror("Fork failed.");
    }

}


int main()
{

	char *path2;
	char *arr [250];
	char *Tokenized ;
	char input[250];
	char *argv[TOKENSIZE];

	while(true)
	{
		// give prompt
		cout<< "shm-shell-> ";
		cin.getline(input,250);

		// call function to break input into arguments
		StrTokenizer(input, argv); 


		// if input was exit then exit else execute command 
		if (strcmp(input, "exit") == 0)  

		{
			break;
		}
		else if (strcmp(input, "\n") == 0){
			continue;
		}
		else if (strcmp(input, " ") == 0){
			continue;
		}
		else if (strcmp(input, "cd") == 0){
			ChangeDirectory(argv);
			continue;
		}
		else if (strcmp(input, "pwd") == 0){
			PrintWorkingDirectory();
			continue;
		}
		else if (strcmp(input, "env") == 0){
			PrintEnvironment();
			continue;
		}
		else if (strcmp(input, "help") == 0){
			PrintHelp();
			continue;
		}
		else if (strcmp(input, "clear") == 0){
			ClearScreen();
			continue;
		}
		else if (strcmp(input, "PATH") == 0){
			GetEnv();
			continue;
		}
		else if (strcmp(input, "find") == 0){
			FindTextInDirectory(argv);
			continue;
		}
		else if (strcmp(input, "runcpp") == 0){
			RunCpp(argv[1]);
			continue;
		}else if (strcmp(input, "filestat") == 0){
			filestat(argv[1]);
			continue;
		}
		else if (strcmp(input, "recieve") == 0){
			recieve();
			continue;
		}
		else if (strcmp(input, "send") == 0){
			send();
			continue;
		}
		else if (strcmp(input, "exroot") == 0){
			transferFilesToRootDirectory(argv[1]);
			continue;
		}
		
		//this function uses execvp to execute command
		myExecvp(argv);
	}
	return 0;
}











