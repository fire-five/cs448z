#ifndef TRI_MESH_H
#define TRI_MESH_H

#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex(glm::vec3 p, glm::vec3 n) : position(p), normal(n) {}
    Vertex() : position(glm::vec3(0.0f, 0.0f, 0.0f)), normal(glm::vec3(0.0f, 0.0f, 0.0f)) {}
};


class TriMesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> faces;

    void setup_data();
    void render(Shader shader);

    void compute_normals();
    virtual void compute_half_vertex(const Vertex v0, const Vertex v1, Vertex &hv);
    void subdivision(GLuint s);
private:
    GLuint VAO, VBO, EBO;
};

#endif