#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) {
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    movement_speed = SPEED;
    mouse_sensitivity = SENSITIVITY;
    zoom = ZOOM;
    this->position = position;
    world_up = up;
    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_parameters();
}


Camera::Camera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat up_x, GLfloat up_y, GLfloat up_z, GLfloat yaw, GLfloat pitch) {
    position = glm::vec3(pos_x, pos_y, pos_z);
    world_up = glm::vec3(up_x, up_y, up_z);
    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_parameters();
}


glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up);
}


void Camera::process_keyboard(CameraMovement direction, GLfloat delta_time) {
    GLfloat velocity = movement_speed * delta_time;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

void Camera::process_mouse_movement(GLfloat xoffset, GLfloat yoffset, GLboolean constrain_pitch) {
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    update_camera_parameters();
}


void Camera::process_mouse_scroll(GLfloat yoffset) {
    zoom -= static_cast<GLfloat>(yoffset);
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}


void Camera::update_camera_parameters() {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}