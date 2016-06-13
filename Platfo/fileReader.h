#ifndef FILEREADER_H_INCLUDED
#define FILEREADER_H_INCLUDED

#include <string>
#include <json/json.h>

class JSONFile {
public:
    JSONFile();

    bool readFile(std::string fileName);
    Json::Value getValue();

private:
    Json::Value fileValue;
    std::string fileName;
};


#endif // FILEREADER_H_INCLUDED
