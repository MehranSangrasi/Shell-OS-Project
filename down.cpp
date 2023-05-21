#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

bool DownloadFile(const std::string& url, const std::string& outputFile)
{
    // Create a command string to execute the wget command
    std::stringstream commandStream;
    commandStream << "wget -O \"" << outputFile << "\" \"" << url << "\"";

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

int main()
{
    std::string url;
    std::cout << "Enter the URL to download: ";
    std::getline(std::cin, url);

    std::string outputFile;
    std::cout << "Enter the output file name: ";
    std::getline(std::cin, outputFile);

    DownloadFile(url, outputFile);

    return 0;
}
