#include "box.h"
#include "cuboid.h"

Box::Box(glm::vec3 center, glm::vec3 color,
    GLfloat length, GLfloat width, GLfloat height): RigidBody(center, color) {
    this->length = length;
    this->width = width;
    this->height = height;
    mesh = Cuboid(length, width, height);
}