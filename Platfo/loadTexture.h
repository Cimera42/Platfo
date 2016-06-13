#ifndef LOADTEXTURE_H_INCLUDED
#define LOADTEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <vector>
#include <string>

GLuint load2DTexture(std::string, bool); ///Loads 2DTexture and returns GLuint ID
unsigned char* load2DTextureData(std::string, int*, int*); ///Loads 2DTexture and returns its data
GLuint load2DTextureByData(unsigned char*, int, int, bool); ///Loads 2DTexture by data and return GLuint ID
GLuint load2DTextureArray(std::vector<std::string>, int); ///Loads 2DTextureArray and returns GLuint ID
GLuint loadCubemapTexture(std::vector<std::string>); ///Loads Cubemap textures from vector of filenames and returns GLuint

#endif // LOADTEXTURE_H_INCLUDED
