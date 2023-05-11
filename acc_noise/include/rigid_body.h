#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "tri_mesh.h"

#include <glm/glm.hpp>

class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 color;
    TriMesh mesh;

    RigidBody(glm::vec3 pos, glm::vec3 color) :
        position(pos), color(color) {}

    virtual void simulate(GLfloat dt) = 0; 
private:
};

#endif