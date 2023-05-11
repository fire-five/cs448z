#include "sphere.h"
#include "icosahedron.h"

Sphere::Sphere(GLfloat r) {
    radius = r;
    Icosahedron icosahedron(r);
    icosahedron.subdivision(5);
    vertices = icosahedron.vertices;
    faces = icosahedron.faces;
    setup_data();
}