#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cmath>

// relative to the observer(i.e. who holds the camera)
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// default configurations
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 2.5f;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 45.0f;

class Camera {
public:
    // parameters 
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    GLfloat movement_speed;
    GLfloat mouse_sensitivity;
    GLfloat zoom;
    // euler angles
    GLfloat yaw;
    GLfloat pitch;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
    // constructor with scalar values
    Camera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat up_x, GLfloat up_y, GLfloat up_z, GLfloat yaw, GLfloat pitch);
    glm::mat4 get_view_matrix();
    void process_keyboard(CameraMovement direction, GLfloat delta_time);
    void process_mouse_movement(GLfloat xoffset, GLfloat yoffset, GLboolean constrain_pitch = true);
    void process_mouse_scroll(GLfloat yoffset);
private:
    void update_camera_parameters();
};

#endif