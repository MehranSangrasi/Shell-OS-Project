// command_line_arguments.cpp  

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>


#include <sys/types.h>
#include <sys/wait.h>


#define TOKENSIZE 100


using namespace std;
void  StrTokenizer(char *line, char **argv);
void  myExecvp(char **argv);
int GetEnv();



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
		cout<< "cwushell-> ";
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

		//this function uses execvp to execute command
		myExecvp(argv);
	}
	return 0;
}


//executes command using execvp
void  myExecvp(char **argv)
{
	pid_t  pid;
	int    status;
	int childStatus;

	// fork to create a child process
	pid = fork();
	if(pid == 0) // inside the child process run the command we want to execute
	{
		childStatus = execvp(*argv, argv);

		//*argv is name of command e.g ls
		//argv are all arguments required for it
		//execvp searches for the executable file e.g 'ls' in the PATH environment variable

		if (childStatus < 0){
			cout<<"ERROR:wrong input"<<endl;
		}
		exit(0);

	}
	else if(pid < 0)
	{
		cout<< "somthing went wrong!"<<endl;
	}
	else //parent process calls wait to wait for child
	{   
		int status;
		waitpid(pid, &status , 0);

	}
}



// Breaks input string into seperate arguments
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


// Gets the PATH environment variable
// Not Called in the original code
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











