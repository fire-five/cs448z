#ifndef PARTICLE_H
#define PARTICLE_H

#include "tri_mesh.h"

#include <glad/glad.h>
#include <glm/glm.hpp>


class Particle {
public:
    Particle(GLfloat density, GLfloat modulus, GLfloat ratio, GLfloat r, glm::vec3 pos, glm::vec3 vel, glm::vec3 color);
 
    void compute_force();
    void simulate(GLfloat dt);


    GLfloat density;
    GLfloat Young_modulus;
    GLfloat Poisson_ratio;
    GLfloat mass;
    GLfloat radius;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 color;
    TriMesh mesh;

    // CCD
    glm::vec3 last_position;
    // bounding box for speeding up 
    glm::vec3 minp;
    glm::vec3 maxp;

private:
};





#endif