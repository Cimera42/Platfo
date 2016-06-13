#include "fileReader.h"
#include "logger.h"
#include <iostream>
#include <fstream>
//#include <sstream>

JSONFile::JSONFile() {
    fileName = "";
    fileValue = "";
}

bool JSONFile::readFile(std::string inFile)
{
    fileName = inFile;
    std::ifstream file;

    file.open(fileName);
    if(file.is_open())
    {
        //Overload, alternatively could use
        //Json::Reader to parse the file
        file >> fileValue;

        file.close();
        return true;
    }
    return false;
}

Json::Value JSONFile::getValue()
{
    return fileValue;
}
