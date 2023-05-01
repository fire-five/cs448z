#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "tri_mesh.h"

#include <glm/glm.hpp>

class RigidBody {
public:
    glm::vec3 center_pos;
    glm::vec3 object_color;
    TriMesh mesh;

    RigidBody(glm::vec3 center, glm::vec3 color) : center_pos(center), object_color(color) {}

    virtual void simulate(GLfloat dt) = 0; 
private:
};

#endif