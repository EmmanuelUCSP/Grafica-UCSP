#ifndef CAM_H
#define CAM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <vector>
#include <iostream>

const float SPEED = 10.5f;
const float SENSITIVITY = 0.8f;
const float ZOOM = 45.0f;
namespace camera_movement {
    enum CamMov {FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN};
}


bool is_equal(float a, float b, float epsilon = 0.0001f){
    return std::abs(std::abs(a) - std::abs(b)) < epsilon;
}

using glm::vec3;

std::vector<vec3> rl_vals{
    vec3(1.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(-1.0f, 0.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f)
};
std::vector<vec3> ud_vals{
    vec3(0.0f, 1.0f, 0.0f),
    vec3(0.0f, 0.0f, -1.0f),
    vec3(0.0f, -1.0f, 0.0f),
    vec3(0.0f, 0.0f, 1.0f)
};

class Camera{
    vec3 position;
    vec3 front, up, right, worldUp;
    vec3 up2, right2;
    float frontier;
    float speed;
    float sensitivity;
    float zoom;
    int rl_idx=0, ud_idx=0;
public:
    Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f)) 
            : worldUp(up), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM){
            this->position = position;
            front = vec3(0.0f, 0.0f, -1.0f);
            this->updateCameraVectors();
            up2 = vec3(0.0f, 1.0f, 0.0f);
            right2 = vec3(1.0f, 0.0f, 0.0f);
            frontier= position.z;
    }

    void updateCameraVectors(){
        vec3 front = vec3(-this->position);
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }

    void updateCameraVectors2(camera_movement::CamMov direction){
        if (direction == camera_movement::RIGHT){
            if (is_equal(position.x, position.z)){
                ++rl_idx;
                right2 = rl_vals[rl_idx%4];
            }
        }
        if (direction == camera_movement::LEFT){
            if (is_equal(position.x, position.z)){
                rl_idx+=3;
                right2 = rl_vals[rl_idx%4];
            }
        }
        if (direction == camera_movement::UP){
            if (is_equal(position.y, position.z)){
                ++ud_idx;
                up2 = ud_vals[ud_idx%4];
            }
        }
        if (direction == camera_movement::DOWN){
            if (is_equal(position.y, position.z)){
                ud_idx+=3;
                up2 = ud_vals[ud_idx%4];
            }
        }
    }

    void processMouseScroll(float yoffset){
        this->zoom -= yoffset;
        if (this->zoom <= 1.0f)
            this->zoom = 1.0f;
        if (this->zoom >= 45.0f)
            this->zoom = 45.0f;
    }

    void processKeyboard(camera_movement::CamMov direction, float deltaTime){
        float velocity = this->speed * deltaTime;
        updateCameraVectors();
        if (direction == camera_movement::FORWARD && valid_position(direction)){
            this->position += this->front * velocity;
        }
        if (direction == camera_movement::BACKWARD){
            this->position -= this->front * velocity;
        }
        if (direction == camera_movement::LEFT){
            this->position -= this->right * velocity;
        }
        if (direction == camera_movement::RIGHT){
            this->position += this->right * velocity;
        }
        if (direction == camera_movement::UP && valid_position(direction)){
            this->position += this->up * velocity;
        }
        if (direction == camera_movement::DOWN && valid_position(direction)){
            this->position -= this->up * velocity;
        }
    }

    void processKeyboard2(camera_movement::CamMov direction, float deltaTime){
        float velocity = 0.2f;
        if (direction == camera_movement::LEFT){
            this->position -= this->right2 * velocity;
        }
        if (direction == camera_movement::RIGHT){
            this->position += this->right2 * velocity;
        }
        if (direction == camera_movement::UP){
            this->position += this->up2 * velocity;
        }
        if (direction == camera_movement::DOWN){
            this->position -= this->up2 * velocity;
        }
        updateCameraVectors2(direction);
    }

    bool valid_position(camera_movement::CamMov direction){
        if (direction == camera_movement::FORWARD){
            return (glm::distance(this->position, vec3(0.0f)) > 0.5f);
        }    
        if (direction == camera_movement::UP || direction == camera_movement::DOWN){
            vec3 v = (direction == camera_movement::UP) ? vec3(0.f, 1.f, 0.f) : vec3(0.f, -1.f, 0.f);
            vec3 u = glm::normalize(this->position);
            float angle = glm::acos(glm::dot(u, v) / (glm::length(u) * glm::length(v)));
            return (glm::degrees(angle) > 1.0f);
        }
        return true;
    }

    glm::mat4 getViewMatrix(){
        return glm::lookAt(this->position, vec3(0.0f), this->up);
    }

    float get_zoom(){
        return this->zoom;
    }

    vec3 get_position(){
        return this->position;
    }
};

#endif