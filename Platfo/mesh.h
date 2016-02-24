#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#include <string>

class Mesh
{
    public:
        Mesh();
        virtual ~Mesh();

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        int bufSize;

        GLuint VAO = 0;
        GLuint vertexBuffer;
        GLuint uvBuffer;
        GLuint normalBuffer;
        GLuint indexBuffer;

        void createVAO();
        void genBuffers();
        void loadModel(std::string filepath);
        void loadWithVectors(std::vector<glm::vec3> inVertices,
                         std::vector<glm::vec2> inUvs,
                         std::vector<glm::vec3> inNormals,
                         std::vector<unsigned int> inIndices);
};

#endif // MESH_H_INCLUDED
