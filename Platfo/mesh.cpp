#include "mesh.h"
#include "openGLFunctions.h"
#include "logger.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

Mesh::Mesh()
{

}
Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteBuffers(1, &normalBuffer);
}

void Mesh::createVAO()
{
    glGenVertexArrays(1, &VAO);
    glSetBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        //Enable vertex buffer
        int vertLoc = 0;
        glEnableVertexAttribArray(vertLoc);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //attribute, size, type, is normalised?, stride, offset

        int uvLoc = 1;
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0); //attribute, size, type, is normalised?, stride, offset

        int normLoc = 2;
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glEnableVertexAttribArray(normLoc);
        glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, 0, 0); //attribute, size, type, is normalised?, stride, offset
    glSetBindVertexArray(0);
}

void Mesh::genBuffers()
{
    //Generate vertex buffer using standard data
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    //Set up uv buffer for data stream
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    //Set up normal buffer for data stream
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    //Generate index buffer using standard data
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    bufSize = indices.size();
}

void Mesh::loadModel(std::string filepath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath.c_str(),
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_Triangulate |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType);

    if(!scene)
    {
        Logger() << "Could not load model. Error importing" << std::endl;
        return;
    }

    for(int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* assimpMesh = scene->mMeshes[i];

        for(int j = 0; j < assimpMesh->mNumFaces; j++)
        {
            aiFace& assimpFace = assimpMesh->mFaces[j];

            for(int k = 0; k < assimpFace.mNumIndices; k++)
            {
                indices.push_back(assimpFace.mIndices[k]);
            }
        }
        for(int j = 0; j < assimpMesh->mNumVertices; j++)
        {
            aiVector3D vertex = assimpMesh->mVertices[j];
            vertices.push_back(glm::vec3(vertex.x,vertex.y,vertex.z));

            aiVector3D uv = assimpMesh->mTextureCoords[0][j];
            uvs.push_back(glm::vec2(uv.x,uv.y));

            aiVector3D normal = assimpMesh->mNormals[j];
            normals.push_back(glm::vec3(normal.x,normal.y,normal.z));
        }
    }

    genBuffers();
}

void Mesh::loadWithVectors(std::vector<glm::vec3> inVertices, std::vector<glm::vec2> inUvs, std::vector<glm::vec3> inNormals, std::vector<unsigned int> inIndices)
{
    vertices.swap(inVertices);
    uvs.swap(inUvs);
    normals.swap(inNormals);
    indices.swap(inIndices);

    genBuffers();
}
