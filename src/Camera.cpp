#include "engine/Camera.h"
#include "engine/shared/Time.h"

Camera::Camera(CameraConfig &config) 
    : front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    pitch(0.0f), 
    yaw(-90.0f)
{
    worldUp     = up;
    position    = config.position;
    speed       = config.speed;
    sensitivity = config.sens;

    UpdateCameraVectors();
}

void Camera::KeyboardInput(CameraDirection direction, double deltaTime)
{
    auto velocity = speed * static_cast<float>(deltaTime);

    if (direction == CameraDirection::FORWARD)
    {
        position += front * velocity;
    }
    if (direction == CameraDirection::BACKWARD)
    {
        position -= front * velocity;
    }
    if (direction == CameraDirection::LEFT)
    {
        position -= right * velocity;
    }
    if (direction == CameraDirection::RIGHT)
    {
        position += right * velocity;
    }
    if (direction == CameraDirection::UP)
    {
        position += worldUp * velocity;
    }
    if (direction == CameraDirection::DOWN)
    {
        position -= worldUp * velocity;
    }
}

void Camera::MouseMovingInput(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 newFront(0.0f);
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

bool Camera::AnimatePos(glm::vec3 &start, const glm::vec3 end, double duration, double startTime)
{
    auto currentTime = Time::GetMainTime();
    auto elapsedTime = currentTime - startTime;
    auto step = static_cast<float>(elapsedTime / duration);
    step = glm::clamp(step, 0.0f, 1.0f);
    auto newPos = glm::mix(start, end, step);
    setPosition(newPos);
    return elapsedTime >= duration;
}

glm::mat4 Camera::getViewMatrix() const 
{
    return glm::lookAt(position, position + front, worldUp);
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(glm::vec3 &value)
{
    position = value;
}

void Camera::setSpeed(float &value)
{
    speed = value;
}

void Camera::setSensitivity(float &value)
{
    sensitivity = value;
}

