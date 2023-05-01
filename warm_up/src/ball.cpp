#include "ball.h"
#include "sphere.h"


Ball::Ball(glm::vec3 center, glm::vec3 color, GLfloat r, glm::vec3 v, GLfloat m) : RigidBody(center, color){
    radius = r;
    velocity = v;
    mass = m;
    mesh = Sphere(r);
}


void Ball::simulate(GLfloat dt) {
    const glm::vec3 G(0.0f, -9.8f, 0.0f);
    velocity += G * dt;
    center_pos += velocity * dt;
}