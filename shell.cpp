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
#include <fstream>
#include <sstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>


#define TOKENSIZE 100
namespace fs = std::filesystem;

using namespace std;
void  StrTokenizer(char *line, char **argv);
void  myExecvp(char **argv);
void GetEnv();

namespace fs = std::filesystem;

void powerRename(const std::string& folderPath, const std::string& extension, const std::string& newname)
{
    int counter = 1;
    std::stringstream ss;

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == extension)
        {
            ss.str(""); // Clear the stringstream
            ss << folderPath << newname << "_" << counter << extension;

            fs::rename(entry.path(), ss.str());

            std::cout << "Renamed " << entry.path().filename() << " to " << ss.str() << std::endl;

            counter++;
        }
    }
}


// Swap the content of two files
void swapContentsOfFile (const std::string& filePath1, const std::string& filePath2)
{
	// Open the first file
	std::ifstream file1(filePath1);
	if (!file1)
	{
		std::cerr << "Failed to open file: " << filePath1 << std::endl;
		return;
	}

	// Open the second file
	std::ifstream file2(filePath2);
	if (!file2)
	{
		std::cerr << "Failed to open file: " << filePath2 << std::endl;
		return;
	}

	// Read the contents of the first file
	std::stringstream file1Contents;
	file1Contents << file1.rdbuf();

	// Read the contents of the second file
	std::stringstream file2Contents;
	file2Contents << file2.rdbuf();

	// Close the files
	file1.close();
	file2.close();

	// Open the first file in write mode
	std::ofstream file1Write(filePath1);
	if (!file1Write)
	{
		std::cerr << "Failed to open file: " << filePath1 << std::endl;
		return;
	}

	// Open the second file in write mode
	std::ofstream file2Write(filePath2);
	if (!file2Write)
	{
		std::cerr << "Failed to open file: " << filePath2 << std::endl;
		return;
	}

	// Write the contents of the first file to the second file
	file2Write << file1Contents.str();

	// Write the contents of the second file to the first file
	file1Write << file2Contents.str();

	// Close the files
	file1Write.close();
	file2Write.close();
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

void GetEnv()
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
void groupFilesByExtension(const std::string& directoryPath)
{
    for (const auto& entry : fs::recursive_directory_iterator(directoryPath))
    {
        if (fs::is_regular_file(entry.path()))
        {
            const std::string extension = entry.path().extension().string();
            std::string targetDirectory = directoryPath + "/" + extension;

            // Remove the dot (.) from the extension if it exists
            if (!extension.empty() && extension[0] == '.')
            {
                targetDirectory = directoryPath + "/" + extension.substr(1);
            }

            if (!fs::exists(targetDirectory))
            {
                fs::create_directory(targetDirectory);
            }

            std::string targetFilePath = targetDirectory + "/" + entry.path().filename().string();
            fs::rename(entry.path(), targetFilePath);

            std::cout << "Moved file: " << entry.path() << " to: " << targetFilePath << std::endl;
        }
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
bool DownloadFile(const std::string& url, const std::string& outputFile)
{
    // Create a command string to execute the wget command
    std::stringstream commandStream;
    commandStream << "wget -O \"" << outputFile << "\" \"" << url << "\"";  // wget system call

    // Convert the command string to a C-style string
    std::string command = commandStream.str();
    const char* commandStr = command.c_str();

    // Execute the wget command
    int result = std::system(commandStr);

    // Check the result of the wget command
    if (result == 0)
    {
        std::cout << "File downloaded successfully" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Failed to download file" << std::endl;
        return false;
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
	cout << "help " << endl;
	cout << "clear" << endl;
	cout << "cd <directory>" << endl;
	cout << "pwd" << endl;
	cout << "env " << endl;
	cout << "find <text> <directory>" << endl;
	cout << "runcpp <filename>" << endl;
	cout << "filestat <filename>" << endl;
	cout << "recieve " << endl;
	cout << "send" << endl;
	cout << "exroot <directory>" << endl;
	cout << "download <url> <output file>" << endl;
	cout << "powerRename <directory> <extension> <new name>" << endl;
	cout << "deleteEmpty <directory>" << endl;
	cout << "orgext <directory>" << endl;
	cout << "ps <process id>" << endl;
	cout << "swap <filename1> <filename2>" << endl;


}

void GetProcessUsage(pid_t processId)
{
    std::string statFile = "/proc/" + std::to_string(processId) + "/stat";
    std::ifstream file(statFile);
    if (!file)
    {
        std::cerr << "Failed to open process stat file\n";
        return;
    }

    // Read process stat file
    std::string line;
    std::getline(file, line);

    std::istringstream iss(line);
    std::string pid, comm, state, ppid, pgrp;
    iss >> pid >> comm >> state >> ppid >> pgrp;

    // Get process memory usage
    std::string statusFile = "/proc/" + std::to_string(processId) + "/status";
    std::ifstream memFile(statusFile);
    if (!memFile)
    {
        std::cerr << "Failed to open process status file\n";
        return;
    }

    std::string memLine;
    while (std::getline(memFile, memLine))
    {
        if (memLine.substr(0, 6) == "VmSize")
        {
            std::istringstream memIss(memLine);
            std::string label;
            long memSize;
            memIss >> label >> memSize;

            std::cout << "Memory Usage: " << memSize << " kB\n";
            break;
        }
    }

    // Get process CPU usage
    long hz = sysconf(_SC_CLK_TCK);
    long utime, stime;
    iss >> utime >> stime;

    double cpuUsage = (utime + stime) / static_cast<double>(hz);
    std::cout << "CPU Usage: " << cpuUsage << " seconds\n";
}

void FindTextInDirectory(char *argv[])
{
    const char *searchText = argv[1];
    const char *directoryPath = argv[2];
	if (argv[1] == NULL || argv[2] == NULL)
	{	
		cout << "ERROR: No search text or directory specified" << endl;
		return;
	}

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

void deleteEmptyFolders(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (fs::is_directory(entry.path()))
        {
            if (fs::is_empty(entry.path()))
            {
                std::cout << "Deleting empty folder: " << entry.path() << std::endl;
                fs::remove(entry.path());
            }
            else
            {
                deleteEmptyFolders(entry.path().string());
            }
        }
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
		else if (strcmp(input, "download") == 0){
			DownloadFile(argv[1], argv[2]);
			continue;
		}
		else if (strcmp(input, "prename") == 0){
			// argv[1] might contain spaces
			std::string directoryPath = argv[1];
			int not_null=0;
			for (int i=1; i< TOKENSIZE; i++)
			{
				if ( argv[i]!=NULL )
					{not_null++;}
				else 
					{break;}
			}
			for (int i = 2; i < not_null-1; i++)
			{
				if (argv[i] == NULL)
					break;
				directoryPath += " ";
				directoryPath += argv[i];
			}
	
			powerRename(directoryPath, argv[not_null-1], argv[not_null]);

			continue;
		}
		else if (strcmp(input, "deleteempty") == 0){
			std::string directoryPath = argv[1];
			for (int i=2;i<TOKENSIZE;i++)
			{
				if (argv[i] == NULL)
					break;
				else
				{
					directoryPath += " ";
					directoryPath += argv[i];
				}
				deleteEmptyFolders(directoryPath);
			}
			continue;
		}
		else if (strcmp(input, "orgext") == 0){
			std::string directoryPath = argv[1];
			for (int i=2;i<TOKENSIZE;i++)
			{
				if (argv[i] == NULL)
					break;
				else
				{
					directoryPath += " ";
					directoryPath += argv[i];
				}
				groupFilesByExtension(directoryPath);
			continue;
		}}
		else if (strcmp(input, "ps") == 0){
			GetProcessUsage(atoi(argv[1]));
			continue;
		}
		else if (strcmp(input, "swap") == 0){
			swapContentsOfFile(argv[1], argv[2]);
			continue;
		}

		else 
		{
			cout << "ERROR: Command not found" << endl;
			continue;
		}

		
		//this function uses execvp to execute command
		// myExecvp(argv);

	}
	return 0;
}











