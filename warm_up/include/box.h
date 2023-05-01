#ifndef BOX_H
#define BOX_H

#include "rigid_body.h"

class Box : public RigidBody {
public:
    GLuint length, width, height;
    Box(glm::vec3 center, glm::vec3 color, GLfloat length, GLfloat width, GLfloat height);
    void simulate(GLfloat dt) override {}
private:
};


#endif