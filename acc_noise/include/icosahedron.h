#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

#include "tri_mesh.h"

class Icosahedron : public TriMesh {
public:
    GLfloat radius;

    Icosahedron(GLfloat r);

    void compute_half_vertex(const Vertex v0, const Vertex v1, Vertex &hv) override;
};


#endif