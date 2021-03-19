#include "Camera.hpp"
#include <iostream>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) { 
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        //this->cameraFrontDirection = cameraTarget;
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() { 
        return glm::lookAt(this->cameraPosition, this->cameraPosition + this->cameraFrontDirection, glm::normalize(this->cameraUpDirection));
    }

    glm::vec3 Camera::getPosition()
    {
        return this->cameraPosition;
    }

    glm::vec3 Camera::getFrontDirection()
    {
        return this->cameraFrontDirection;
    }

    glm::vec3 Camera::getTarget()
    {
        return this->cameraTarget;
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //std::cout << this->cameraPosition.x << ' ' << this->cameraPosition.y << ' ' << this->cameraPosition.z << std::endl;
        switch (direction)
        {
        case gps::MOVE_FORWARD:
            this->cameraPosition += this->cameraFrontDirection * speed;
            break;
        case gps::MOVE_BACKWARD:
            this->cameraPosition -= this->cameraFrontDirection * speed;
            break;
        case gps::MOVE_RIGHT:
            this->cameraPosition += this->cameraRightDirection * speed;
            break;
        case gps::MOVE_LEFT:
            this->cameraPosition -= this->cameraRightDirection * speed;
            break;
        default:
            break;
        }
    }

    void Camera::moveUp(float speed) {
        this->cameraPosition += this->cameraUpDirection * speed;
    }

    void Camera::moveDown(float speed) {
        this->cameraPosition -= this->cameraUpDirection * speed;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        float pitch_new;
        if (pitch < -89) {
            pitch_new = -89;
        }
        else if (pitch > 89) {
            pitch_new = 89;
        }
        else {
            pitch_new = pitch;
        }

        float x = glm::cos(pitch_new) * glm::cos(yaw);
        float y = glm::sin(pitch_new);
        float z = glm::cos(pitch_new) * glm::sin(yaw);

        glm::vec3 vec = glm::vec3(x, y, z);

        this->cameraFrontDirection = glm::normalize(vec);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));
    }
}