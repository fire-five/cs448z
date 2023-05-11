#include "tri_mesh.h"

#include <cstddef>


void TriMesh::compute_normals() {
    std::vector<GLfloat> total_area(vertices.size(), 0.0);
    for (int i = 0; i < faces.size(); i += 3) {
        GLuint i0 = faces[i], i1 = faces[i+1], i2 = faces[i+2];
        Vertex v0 = vertices[i0];
        Vertex v1 = vertices[i1];
        Vertex v2 = vertices[i2];
        glm::vec3 p01 = v1.position - v0.position;
        glm::vec3 p02 = v2.position - v0.position;
        // normal with area
        glm::vec3 face_normal = glm::cross(p01, p02);
        GLfloat area = glm::length(face_normal);
        vertices[i0].normal += face_normal;
        vertices[i1].normal += face_normal;
        vertices[i2].normal += face_normal;
        total_area[i0] += area;
        total_area[i1] += area;
        total_area[i2] += area;
    }
    for (int i = 0; i < vertices.size(); i ++) {
        vertices[i].normal /= total_area[i];
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }
}


void TriMesh::subdivision(GLuint s) {
    std::vector<Vertex> backup_vertices;
    std::vector<GLuint> backup_faces;

    for (uint i = 0; i < s; i ++) {
        backup_vertices = vertices;
        backup_faces = faces;
        vertices.clear();
        faces.clear();
        GLuint index = 0;
        for (uint j = 0; j < backup_faces.size(); j += 3) {
            Vertex v0 = backup_vertices[backup_faces[j]];
            Vertex v1 = backup_vertices[backup_faces[j + 1]];
            Vertex v2 = backup_vertices[backup_faces[j + 2]];
            Vertex new_v0, new_v1, new_v2;
            compute_half_vertex(v0, v1, new_v0);
            compute_half_vertex(v1, v2, new_v1);
            compute_half_vertex(v0, v2, new_v2);
            vertices.push_back(v0); vertices.push_back(new_v0);
            vertices.push_back(v1); vertices.push_back(new_v1);
            vertices.push_back(v2); vertices.push_back(new_v2);
            faces.push_back(index); faces.push_back(index + 1); faces.push_back(index + 5);
            faces.push_back(index + 1); faces.push_back(index + 2); faces.push_back(index + 3);
            faces.push_back(index + 1); faces.push_back(index + 3); faces.push_back(index + 5);
            faces.push_back(index + 5); faces.push_back(index + 3); faces.push_back(index + 4);
            index += 6;
        }
    }

    compute_normals();
    setup_data();
}


void TriMesh::compute_half_vertex(const Vertex v0, const Vertex v1, Vertex &hv) {
    hv.position = (v0.position + v1.position) * 0.5f;
    auto pos = hv.position;
}


void TriMesh::render(Shader shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void TriMesh::setup_data() {
    // create
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind
    glBindVertexArray(VAO);
    // load data
    // vertex
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        &vertices[0],
        GL_STATIC_DRAW
    );
    // index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        faces.size() * sizeof(GLuint),
        &faces[0],
        GL_STATIC_DRAW
    );
    // set vertex attribute ptr
    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (GLvoid*)0
    );
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, normal)
    );

    glBindVertexArray(0);
}