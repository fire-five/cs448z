#include "collision.h"

#include <AL/al.h>

#include <cmath>
#include <algorithm>
#include <tuple>


auto compare_first = [](const std::tuple<GLfloat, GLuint, GLuint>& a, const std::tuple<GLfloat, GLuint, GLuint>& b) {
    return std::get<0>(a) < std::get<0>(b);
};


void collision_response(Particle *p0, Particle *p1, GLfloat dt, GLfloat ToC, glm::vec3 observer_pos, SoundPlayer *player) {
    // constants
    GLfloat COR = 1.0f;
    GLfloat RHO0 = 1.20f;
    GLfloat C = 330.0f;
    GLfloat SAMPLE_RATE = 60;

    glm::vec3 n = glm::normalize(p0->position - p1->position);
    GLfloat v_pre = glm::dot((p0->velocity - p1->velocity), n);
    if (v_pre >= 0.0f) {
        return;
    }
    // update velocity and position post-collision
    p0->position = p0->last_position + ToC * p0->velocity;
    p1->position = p1->last_position + ToC * p1->velocity;
    glm::vec3 delta_v = n * (0.5f + 0.5f*COR) * v_pre;
    p0->velocity -= delta_v;
    p1->velocity += delta_v;
    p0->position += (dt - ToC) * p0->velocity;
    p1->position += (dt - ToC) * p1->velocity;
    // synthesize accleration noise
    GLfloat m = (p0->mass * p1->mass) / (p0->mass + p1->mass);
    glm::vec3 j0 = n * (1+COR) * m * v_pre; 
    glm::vec3 j1 = -j0;
    GLfloat j = (1+COR) * m * v_pre;
    glm::vec3 d0 = observer_pos - p0->position;
    glm::vec3 d1 = observer_pos - p1->position;
    GLfloat dis0 = glm::length(d0);
    GLfloat dis1 = glm::length(d1);
    GLfloat cos_theta0 = glm::dot(n, d0); 
    GLfloat cos_theta1 = glm::dot(-n, d1);
    GLfloat r = (p0->radius * p1->radius) / (p0->radius + p1->radius);
    GLfloat E = (p0->Young_modulus * p1->Young_modulus) / 
        ((1 - powf(p0->Poisson_ratio, 2)) * p1->Young_modulus +
        (1 - powf(p1->Poisson_ratio, 2)) * p0->Young_modulus);
    GLfloat tao = 2.87f * powf(m * m / (r * E * E * glm::length(v_pre)), 0.2f);
    ALuint sample_num = static_cast<ALuint>(tao * SAMPLE_RATE);
    ALfloat sample_dt = static_cast<ALfloat>(tao / sample_num);
    ALfloat *p0_data = new ALfloat[sample_num];
    ALfloat *p1_data = new ALfloat[sample_num];
    ALfloat t = 0.0f;
    for (ALuint i = 0; i < sample_num; i ++, t += sample_dt) {
        ALfloat t0 = t - dis0 / C;
        ALfloat t1 = t - dis1 / C;
        ALfloat power0 = -6.0f/powf(tao, 2) * powf(t0 - 0.5 * tao, 2);
        ALfloat s0_dot = 2.0f * power0 * exp(power0); 
        p0_data[i] = 0.25f * RHO0 * powf(p0->radius, 3) * cos_theta0 / (C * dis0) *
            M_PI / (p0->mass * tao) * j * s0_dot;
        ALfloat power1 = -6.0f/powf(tao, 2) * powf(t1 - 0.5 * tao, 2);
        ALfloat s1_dot = 2.0f * power1 * exp(power1); 
        p1_data[i] = 0.25f * RHO0 * powf(p1->radius, 3) * cos_theta1 / (C * dis1) *
            M_PI / (p0->mass * tao) * j * s1_dot;
    }
    player->add_sound(p0_data, sample_num, SAMPLE_RATE, static_cast<GLfloat*>(&(p0->position.x)));
    player->add_sound(p1_data, sample_num, SAMPLE_RATE, static_cast<GLfloat*>(&(p1->position.x)));
}


void handle_collisions(Box *box, std::vector<Particle*> particle_system, GLfloat dt, glm::vec3 observer_pos, SoundPlayer *player) {
    std::vector<std::tuple<GLfloat, GLuint, GLuint>> x_coords;
    for (GLuint i = 0; i < particle_system.size(); i ++) {
        Particle *p = particle_system[i];
        particle_box_collision(box, p, dt);
        x_coords.push_back(
            std::tuple<GLfloat, GLuint, GLuint>(p->minp.x, i, 0)
        );
        x_coords.push_back(
            std::tuple<GLfloat, GLuint, GLuint>(p->maxp.x, i, 1)
        );
    }
    sort(x_coords.begin(), x_coords.end(), compare_first);
    std::vector<GLuint> x_neighbors;
    for (auto xt: x_coords) {
        GLfloat x_coord = std::get<0>(xt);
        GLuint id = std::get<1>(xt);
        GLuint flag = std::get<2>(xt);
        if (flag == 0) {
            x_neighbors.push_back(id);
        }
        else if (flag == 1) {
            auto it = std::find(x_neighbors.begin(), x_neighbors.end(), id);
            if (it != x_neighbors.end()) {
                // collisions
                Particle *p0 = particle_system[*it];
                std::vector<Particle*> neighbors;
                for (GLuint i = 0; i < x_neighbors.size(); i ++) {
                    if (x_neighbors[i] == *it) {
                        continue;
                    }
                    else {
                        Particle *p = particle_system[x_neighbors[i]];
                        if ((p->maxp.y < p0->minp.y) || (p->maxp.z < p0->minp.z)) {
                            continue;
                        }
                        if ((p->minp.y > p0->maxp.y) || (p->minp.z > p0->maxp.z)) {
                            continue;
                        }
                        else {
                            neighbors.push_back(p);
                        }
                    }
                }
                for (auto p : neighbors) {
                    GLfloat ToC = particle_pair_collision(*p0, *p, dt);
                    if (ToC >= 0.0f) {
                        collision_response(p0, p, dt, ToC, observer_pos, player);
                    }
                }
                x_neighbors.erase(it);
            }
        }
    }
}


GLfloat particle_box_collision(Box *box, Particle *p, GLfloat dt) {
    GLfloat box_right = box->position.x + box->width * 0.5;
    GLfloat box_left = box->position.x - box->width * 0.5;
    GLfloat box_up = box->position.y + box->height * 0.5;
    GLfloat box_down = box->position.y - box->height * 0.5;
    GLfloat box_front = box->position.z + box->length * 0.5;
    GLfloat box_back = box->position.z - box->length * 0.5;
    glm::vec3 dv(0.0f, 0.0f, 0.0f);
    // force position in box
    if (p->position.x >= box_right - p->radius) {
        p->position.x = box_right - p->radius;
        dv.x += -2.0 * p->velocity.x;
    }
    if (p->position.x <= box_left + p->radius) {
        p->position.x = box_left + p->radius;
        dv.x += -2.0 * p->velocity.x;
    }
    if (p->position.y >= box_up - p->radius) {
        p->position.y = box_up - p->radius;
        dv.y += -2.0 * p->velocity.y;
    }
    if (p->position.y <= box_down + p->radius) {
        p->position.y = box_down + p->radius;
        dv.y += -2.0 * p->velocity.y;
    }
    if (p->position.z >= box_front - p->radius) {
        p->position.z = box_front - p->radius;
        dv.z += -2.0 * p->velocity.z;
    }
    if (p->position.z <= box_back + p->radius) {
        p->position.z = box_back + p->radius;
        dv.z += -2.0 * p->velocity.z;
    }
    // GLfloat ToC = -1.0f;
    // GLfloat v2 = glm::dot(p->velocity, p->velocity);
    // GLfloat r2 = p->radius * p->radius;
    // GLfloat b, c, tp;
    // tp = (box_right - p->last_position.x) / p->velocity.x;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.x += -2.0 * p->velocity.x;
    // }
    // tp = (box_left - p->last_position.x) / p->velocity.x;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.x += -2.0 * p->velocity.x;
    // }
    // tp = (box_up - p->last_position.y) / p->velocity.y;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.y += -2.0 * p->velocity.y;
    // }
    // tp = (box_down - p->last_position.y) / p->velocity.y;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.y += -2.0 * p->velocity.y;
    // }
    // tp = (box_front - p->last_position.z) / p->velocity.z;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.z += -2.0 * p->velocity.z;
    // }
    // tp = (box_back - p->last_position.z) / p->velocity.z;
    // b = -2.0f * v2 * tp;
    // c = tp * tp * v2 - r2; 
    // ToC = first_time_of_collision(v2, b, c, dt);
    // if (ToC >= 0.0f) {
    //     p->position -= p->velocity * (dt - ToC);
    //     dv.z += -2.0 * p->velocity.z;
    // }
    
    p->velocity += dv;
    return glm::length(dv) * p->mass;
}


GLfloat particle_pair_collision(Particle p0, Particle p1, GLfloat dt) {
    glm::vec3 dv = p0.velocity - p1.velocity;
    glm::vec3 dx = p0.last_position - p1.last_position;
    GLfloat a = glm::dot(dv, dv);
    GLfloat b = 2.0f * glm::dot(dx, dv);
    GLfloat c = glm::dot(dx, dx) - powf(p0.radius + p1.radius, 2);
    return first_time_of_collision(a, b, c, dt); 
}


GLfloat first_time_of_collision(GLfloat a, GLfloat b, GLfloat c, GLfloat dt) {
    GLfloat D = powf(b, 2) - 4.0f * a * c;
    if (D >= 0.0f) {
        // Numerically stable method for quadratic equation
        GLfloat q = b >= 0.0f ? -0.5 * (b + sqrtf(D)) : -0.5 * (b - sqrtf(D));
        GLfloat t0 = q / a;
        GLfloat t1 = c / q;
        if (t0 > t1) {
            GLfloat tmp = t0;
            t0 = t1;
            t1 = tmp;
        }
        if (t0 >= 0.0f && t0 <= dt) {
            return t0;
        }
        else if (t1 >= 0.0f && t1 <= dt) {
            return t1;
        }
        else {
            return -1.0f;
        }
    }
    else {
        return -1.0f;
    }
}

