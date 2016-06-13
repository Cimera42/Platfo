#include "loadShader.h"
#include "logger.h"
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <map>

void loadShaderPart(GLuint partID, const char * path)
{
    std::string shaderCode;
    std::ifstream stream(path, std::ios::in);
    if(stream.is_open())
    {
        std::string line;
        while(std::getline(stream, line))
            shaderCode += "\n" + line; //HERERERERE is an error
        stream.close();
    }

    GLint compileResult = GL_FALSE;
    int compileLogLength;

    const char * codePointer = shaderCode.c_str();
    glShaderSource(partID, 1, &codePointer, NULL);
    glCompileShader(partID);

    glGetShaderiv(partID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(partID, GL_INFO_LOG_LENGTH, &compileLogLength);
    std::vector<char> compileLog(compileLogLength);
    glGetShaderInfoLog(partID, compileLogLength, NULL, &compileLog[0]);
    if (compileLog.size() > 1)
        Logger() << compileLog.data() << std::endl;

    std::vector<char>().swap(compileLog);
}

void createProgram(GLuint progID)
{
    try
    {
        glLinkProgram(progID);
        GLint compileResult = 0;
        int compileLogLength = 0;

            glGetProgramiv(progID, GL_LINK_STATUS, &compileResult);
            glGetProgramiv(progID, GL_INFO_LOG_LENGTH, &compileLogLength);
            std::vector<char> compileLog(compileLogLength);
            glGetProgramInfoLog(progID, compileLogLength, NULL, &compileLog[0]);
            if (compileLog.size() > 1)
                Logger() << compileLog.data() <<std::endl;

            std::vector<char>().swap(compileLog);
    }
    catch (std::bad_alloc bad)
    {
        std::cerr<<"Bad allocation from compileLog."<<std::endl;
    }
}

GLuint loadShader(const char * vertPath, const char * fragPath)
{
    Logger()<< "Creating Shader" << std::endl;
    Logger() <<"Vertex: " << vertPath << std::endl;
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        loadShaderPart(vertID, vertPath);

    Logger() <<"Fragment: "<< fragPath << std::endl;
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
        loadShaderPart(fragID, fragPath);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);

    createProgram(programID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);

    return programID;
}

GLuint loadShaderG(const char * vertPath, const char * fragPath, const char * geoPath)
{
    Logger()<< "Creating Shader" << std::endl;
    Logger() <<"Vertex: " << vertPath << std::endl;
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        loadShaderPart(vertID, vertPath);

    Logger() <<"Fragment: "<< fragPath << std::endl;
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
        loadShaderPart(fragID, fragPath);

    Logger() <<"Geometry: "<< geoPath << std::endl;
    GLuint geoID = glCreateShader(GL_GEOMETRY_SHADER);
        loadShaderPart(geoID, geoPath);

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertID);
    glAttachShader(programID, fragID);
    glAttachShader(programID, geoID);

    createProgram(programID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);
    glDeleteShader(geoID);

    return programID;
}

GLuint deleteShader(GLuint inShader)
{
    glDeleteProgram(inShader);
}
