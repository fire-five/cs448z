#include "camera.h"
#include "shader.h"
#include "box.h"
#include "particle.h"
#include "collision.h"
#include "sound_player.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <sndfile.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);


// basic settings
// screen size
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;
const GLchar project_name[] = "Warm Up";
// camera
glm::vec3 observer_pos(0.0f, 0.0f, 8.0f);
Camera camera(observer_pos);
GLfloat last_x = SCR_WIDTH / 2.0f;
GLfloat last_y = SCR_HEIGHT / 2.0f;
GLboolean first_mouse = true;
// timing
GLfloat delta_time = 0.0f;
GLfloat last_frame = 0.0f;
// lighting
glm::vec3 light_pos(0.0f, 0.0f, 3.0f);


int main() {
    // glfw: init & config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, project_name, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // config global opengl state
    glEnable(GL_DEPTH_TEST);

    const char *filename = "../sound/sound.wav";
    glm::vec3 lookat = camera.position + camera.front;
    ALfloat ort[6];
    ort[0] = lookat.x; ort[1] = lookat.y; ort[2] = lookat.z; 
    ort[3] = camera.up.x; ort[4] = camera.up.y; ort[5] = camera.up.z; 
    SoundPlayer *player = new SoundPlayer(
        static_cast<ALfloat *>(&(observer_pos.x)),
        ort
    );


    // build and compile shader
    Shader phone_shader("../shader/phone.vert", "../shader/phone.frag");

    // set up box and particle system
    Box *box = new Box(
        glm::vec3(0.0f, 0.0f, -0.5f),
        glm::vec3(0.5f, 0.5f, 0.31f),
        4.0f, 4.0f, 5.0f
    );
    // materials: steel ceramic glass plastic
    GLfloat densities[4] = {8940.0f, 2700.0f, 2700.0f, 1200.0f};
    GLfloat modulus[4] = {123.4f, 72.0f, 62.0f, 2.4f};
    GLfloat ratios[4] = {0.34f, 0.19f, 0.20f, 0.37f};
    glm::vec3 colors[4] = {
        glm::vec3(0.70196f, 0.70196f, 0.0f),
        glm::vec3(0.0f, 0.8f, 0.6f),
        glm::vec3(0.88235f, 0.91765f, 0.91765f),
        glm::vec3(0.0f, 0.6f, 1.0f),
    };
    std::vector<Particle*> particle_system;
    // random
    for (GLuint i = 0; i < 30; i ++) {
        GLfloat x = -2.0f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(4.0f)));
        GLfloat y = 2.4f;
        GLfloat z = -2.5f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(4.0f)));
        GLfloat r = 0.1f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(0.15f)));
        GLfloat vx = -10.0f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(20.0f)));
        GLfloat vy = -10.0f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(10.0f)));
        GLfloat vz = -10.0f + static_cast<GLfloat>(rand()) / (static_cast<GLfloat>(RAND_MAX/(20.0f)));
        GLint type = static_cast<GLint>(rand() % 4);
        Particle *p = new Particle(
            densities[type],
            modulus[type],
            ratios[type],
            r,
            glm::vec3(x, y, z),
            glm::vec3(vx, vy, vz),
            colors[type]
        );
        particle_system.push_back(p);
    }
    

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        GLfloat current_frame = static_cast<GLfloat>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // simulate
        for (auto particle: particle_system) {
            particle->simulate(delta_time);
        }
        // collision
        handle_collisions(box, particle_system, delta_time, observer_pos, player);
        // play sound
        player->play();

        // input
        processInput(window);
        observer_pos = camera.position;

        // render
        glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // set shader
        phone_shader.use();
        phone_shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        phone_shader.setVec3("lightPos", light_pos);
        phone_shader.setVec3("viewPos", camera.position);
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.get_view_matrix();
        phone_shader.setMat4("projection", projection);
        phone_shader.setMat4("view", view);

        // box
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, box->position);
        phone_shader.setMat4("model", model);
        phone_shader.setVec3("objectColor", box->color);
        box->mesh.render(phone_shader);
        // particles
        for (auto particle: particle_system) {
            // world transformation
            model = glm::mat4(1.0f);
            model = glm::translate(model, particle->position);
            phone_shader.setMat4("model", model);
            phone_shader.setVec3("objectColor", particle->color);
            // render object
            particle->mesh.render(phone_shader);
        }


        // glfw: swap buffer & poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete player;

    delete box;
    for (auto particle: particle_system) {
        delete particle;
    }
}


// process all input: query GLFW wheter relevant keys are pressed/released this frame
//  and react accordingly
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_keyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_keyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_keyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_keyboard(RIGHT, delta_time);
}


// glfw: whenever the window size changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // viewport should match the new window dimensions
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    GLfloat xpos = static_cast<GLfloat>(xposIn);
    GLfloat ypos = static_cast<GLfloat>(yposIn);
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    GLfloat xoffset = xpos - last_x;
    GLfloat yoffset = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    camera.process_mouse_movement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.process_mouse_scroll(static_cast<GLfloat>(yoffset));
}
