#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

void powerRename(const std::string& folderPath, const std::string& extension)
{
    int counter = 1;
    std::stringstream ss;

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == extension)
        {
            ss.str(""); // Clear the stringstream
            ss << folderPath << "/renamed_" << counter << extension;

            fs::rename(entry.path(), ss.str());

            std::cout << "Renamed " << entry.path().filename() << " to " << ss.str() << std::endl;

            counter++;
        }
    }
}

int main()
{
    std::string folderPath = "/media/mehran/New Volume/Projectr/OS/"; // Replace with the actual folder path
    std::string extension = ".txt"; // Replace with the desired file extension

    powerRename(folderPath, extension);

    return 0;
}
