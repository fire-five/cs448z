#ifndef BOX_H
#define BOX_H

#include "tri_mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>


class Box {
public:
    Box(glm::vec3 pos, glm::vec3 color, GLfloat length, GLfloat width, GLfloat height);


    GLfloat length, width, height;
    glm::vec3 position;
    glm::vec3 color;
    TriMesh mesh;
private:
};


#endif