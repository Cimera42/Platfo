#ifndef TYPECONVERSION_H_INCLUDED
#define TYPECONVERSION_H_INCLUDED

#include <string>

bool stringToBool(std::string s);
int stringToInt(std::string s);
float stringToFloat(std::string s);
std::string intToStr(int a);

/*glm::vec2 doubleToVec2(double a, double b);
glm::vec3 doubleToVec3(double a, double b, double c);
glm::vec4 doubleToVec4(double a, double b, double c, double d);*/

#endif // TYPECONVERSION_H_INCLUDED
