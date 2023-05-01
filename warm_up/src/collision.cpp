#include "collision.h"


GLfloat box_ball_collision(Box *box, Ball *ball) {
    GLfloat box_right = box->center_pos.x + box->width * 0.5;
    GLfloat box_left = box->center_pos.x - box->width * 0.5;
    GLfloat box_up = box->center_pos.y + box->height * 0.5;
    GLfloat box_down = box->center_pos.y - box->height * 0.5;
    GLfloat box_front = box->center_pos.z + box->length * 0.5;
    GLfloat box_back = box->center_pos.z - box->length * 0.5;
    GLfloat ball_x = ball->center_pos.x;
    GLfloat ball_y = ball->center_pos.y;
    GLfloat ball_z = ball->center_pos.z;
    glm::vec3 dv(0.0f, 0.0f, 0.0f);
    // force position in box
    if (fabs(box_right - ball_x) <= ball->radius) {
        ball->center_pos.x = box_right - ball->radius;
        dv.x += -2.0 * ball->velocity.x;
    }
    if (fabs(box_left - ball_x) <= ball->radius) {
        ball->center_pos.x = box_left + ball->radius;
        dv.x += -2.0 * ball->velocity.x;
    }
    if (fabs(box_up - ball_y) <= ball->radius) {
        ball->center_pos.y = box_up - ball->radius;
        dv.y += -2.0 * ball->velocity.y;
    }
    if (fabs(box_down - ball_y) <= ball->radius) {
        ball->center_pos.y = box_down + ball->radius;
        dv.y += -2.0 * ball->velocity.y;
    }
    if (fabs(box_front - ball_z) <= ball->radius) {
        ball->center_pos.z = box_front - ball->radius;
        dv.z += -2.0 * ball->velocity.z;
    }
    if (fabs(box_back - ball_z) <= ball->radius) {
        ball->center_pos.z = box_back + ball->radius;
        dv.z += -2.0 * ball->velocity.z;
    }
    ball->velocity += dv;
    return glm::length(dv) * ball->mass;
}