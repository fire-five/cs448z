#include "box.h"
#include "cuboid.h"


Box::Box(glm::vec3 pos, glm::vec3 color,
    GLfloat length, GLfloat width, GLfloat height) {
    position = pos;
    this->color = color;
    this->length = length;
    this->width = width;
    this->height = height;

    mesh = Cuboid(length, width, height);
}
