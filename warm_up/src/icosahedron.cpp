#include "icosahedron.h"

Icosahedron::Icosahedron(GLfloat r) {
    radius = r;
    const GLfloat H_ANGLE = M_PI / 180 * 72;
    const GLfloat V_ANGLE = atanf(1.0f / 2);

    GLfloat h_angle1 = M_PI / 2 - H_ANGLE / 2;
    GLfloat h_angle2 = M_PI / 2;

    vertices = std::vector<Vertex>(12);

    vertices[0] = Vertex(
        glm::vec3(0.0f, 0.0f, radius),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    for (int i = 1; i <= 5; i ++) {
        GLuint i1 = i, i2 = i + 5;
        GLfloat z = radius * sinf(V_ANGLE);
        GLfloat xy = radius * cosf(V_ANGLE);

        vertices[i1] = Vertex(
            glm::vec3(xy*cosf(h_angle1), xy*sinf(h_angle1), z),
            glm::vec3(0.0f, 0.0f, 0.0f)
        );
        auto v = vertices[i1];
        vertices[i2] = Vertex(
            glm::vec3(xy*cosf(h_angle2), xy*sinf(h_angle2), -z),
            glm::vec3(0.0f, 0.0f, 0.0f)
        );

        GLuint ir1 = (i1 % 5 + 1);
        GLuint ir2 = (i1 % 5 + 6);
        faces.push_back(0); faces.push_back(ir1); faces.push_back(i1);
        faces.push_back(11); faces.push_back(i2); faces.push_back(ir2);
        faces.push_back(ir2); faces.push_back(i1); faces.push_back(ir1);
        faces.push_back(i1); faces.push_back(ir2); faces.push_back(i2);

        h_angle1 -= H_ANGLE;
        h_angle2 -= H_ANGLE;
    }

    vertices[11] = Vertex(
        glm::vec3(0.0f, 0.0f, -radius),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    compute_normals();
    setup_data();
}

void Icosahedron::compute_half_vertex(const Vertex v0, const Vertex v1, Vertex &hv) {
    TriMesh::compute_half_vertex(v0, v1, hv);
    // vertex should sit on sphere
    hv.position = glm::normalize(hv.position);
    hv.position *= radius;
}