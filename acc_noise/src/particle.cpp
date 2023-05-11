#include "particle.h"
#include "sphere.h"

#include <algorithm>


Particle::Particle(GLfloat density, GLfloat modulus, GLfloat ratio, GLfloat r, glm::vec3 pos, glm::vec3 vel, glm::vec3 color) {
    this->density = density;
    Young_modulus = modulus;
    Poisson_ratio = ratio;
    radius = r;
    mass = M_PI * 4.0f / 3.0f * powf(radius, 3);
    position = pos;
    velocity = vel;
    this->color = color;
    force = glm::vec3(0.0f, 0.0f, 0.0f);
    mesh = Sphere(radius); 
    last_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 R(radius, radius, radius);
    minp = position - R;
    maxp = position + R;
}


void Particle::compute_force() {
    // only consider garvity now
    const glm::vec3 G(0.0f, -9.8f, 0.0f);
    force = G * mass;
}


void Particle::simulate(GLfloat dt) {
    glm::vec3 R(radius, radius, radius);
    minp = position - R;
    maxp = position + R;

    last_position = position;

    compute_force();
    velocity += dt * force / mass;
    position += velocity * dt;

    glm::vec3 after_minp = position - R;
    glm::vec3 after_maxp = position + R;
    minp.x = std::min(minp.x, after_minp.x);
    minp.y = std::min(minp.y, after_minp.y);
    minp.z = std::min(minp.z, after_minp.z);
    maxp.x = std::max(maxp.x, after_maxp.x);
    maxp.y = std::max(maxp.y, after_maxp.y);
    maxp.z = std::max(maxp.z, after_maxp.z);
}
