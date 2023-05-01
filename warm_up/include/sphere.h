#ifndef SPHERE_H
#define SPHERE_H

#include "tri_mesh.h"

class Sphere : public TriMesh {
public:
    GLfloat radius;

    Sphere(GLfloat r);
};

#endif
