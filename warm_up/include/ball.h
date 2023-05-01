#ifndef BALL_H
#define BALL_H

#include "rigid_body.h"

#include <glad/glad.h>


class Ball : public RigidBody {
public:
    GLfloat radius;
    glm::vec3 velocity;
    GLfloat mass;

    Ball(glm::vec3 center, glm::vec3 color, GLfloat r, glm::vec3 v, GLfloat m);

    void simulate(GLfloat dt) override;
private:
};


#endif