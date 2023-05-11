#include "cuboid.h"


Cuboid::Cuboid(GLfloat length, GLfloat width, GLfloat height) {
    vertices.push_back(
        Vertex(
            glm::vec3(width/2.0, height/2.0, -length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(width/2.0, height/2.0, length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(width/2.0, -height/2.0, length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(width/2.0, -height/2.0, -length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(-width/2.0, height/2.0, length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(-width/2.0, height/2.0, -length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(-width/2.0, -height/2.0, -length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    vertices.push_back(
        Vertex(
            glm::vec3(-width/2.0, -height/2.0, length/2.0),
            glm::vec3(0.0, 0.0, 0.0)
        )
    );
    // up
    faces.push_back(0); faces.push_back(5); faces.push_back(4);
    faces.push_back(0); faces.push_back(4); faces.push_back(1);
    // back
    faces.push_back(0); faces.push_back(6); faces.push_back(5);
    faces.push_back(0); faces.push_back(3); faces.push_back(6);
    // right
    faces.push_back(0); faces.push_back(1); faces.push_back(2);
    faces.push_back(0); faces.push_back(2); faces.push_back(3);
    // left 
    faces.push_back(5); faces.push_back(7); faces.push_back(4);
    faces.push_back(5); faces.push_back(6); faces.push_back(7);
    // down 
    faces.push_back(3); faces.push_back(7); faces.push_back(6);
    faces.push_back(3); faces.push_back(2); faces.push_back(7);
    // set normal of vertices
    compute_normals();
    setup_data();
}
