#ifndef COLLISION_H
#define COLLISION_H

#include "box.h"
#include "particle.h"
#include "sound_player.h"


void handle_collisions(Box *box, std::vector<Particle*> particle_system, GLfloat dt, glm::vec3 observer_pos, SoundPlayer *player);
GLfloat particle_box_collision(Box *box, Particle *p, GLfloat dt);
GLfloat particle_pair_collision(Particle p0, Particle p1, GLfloat dt);
GLfloat first_time_of_collision(GLfloat a, GLfloat b, GLfloat c, GLfloat dt);


#endif