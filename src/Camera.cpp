#include "engine/Camera.h"
#include "engine/shared/Time.h"
#include <iostream>

Camera::Camera()
{
	CameraConfig config;
	PreInitCamera(config);
}

Camera::Camera(const CameraConfig &config)
{
	PreInitCamera(config);
}

void Camera::PreInitCamera(const CameraConfig &config)
{
	front = glm::vec3(0.0F, 0.0F, -1.0F);
	up = glm::vec3(0.0F, 1.0F, 0.0F);
	pitch = 0.0F;
	yaw = -90.0F;
	worldUp = up;
	position = config.position;
	speed = config.speed;
	sensitivity = config.sens;
	isInputActive = true;
	isEyeLocked = false;

	UpdateCameraVectors();
}

void Camera::KeyboardInput(const CameraDirection direction, double deltaTime)
{
	auto velocity = speed * static_cast<float>(deltaTime);

	switch (direction)
	{
		case CameraDirection::FORWARD:
			position += front * velocity;
			break;
		case CameraDirection::BACKWARD:
			position -= front * velocity;
			break;
		case CameraDirection::LEFT:
			position -= right * velocity;
			break;
		case CameraDirection::RIGHT:
			position += right * velocity;
			break;
		case CameraDirection::UP:
			position += worldUp * velocity;
			break;
		case CameraDirection::DOWN:
			position -= worldUp * velocity;
			break;
		default:
			std::cerr << "Unknown camera direction value!\n";
			break;
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
		if (pitch > 89.0F)
			pitch = 89.0F;
		if (pitch < -89.0F)
			pitch = -89.0F;
	}
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 newFront(0.0F);
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

bool Camera::AnimatePos(glm::vec3 &start, const glm::vec3 end, double duration, double startTime)
{
	auto currentTime = Time::FixedTime();
	auto elapsedTime = currentTime - startTime;
	auto progress = static_cast<float>(elapsedTime / duration);
	progress = glm::clamp(progress, 0.0F, 1.0F);
	auto newPos = glm::mix(start, end, progress);
	setPosition(newPos);
	return elapsedTime >= duration;
}

bool Camera::AnimatePath(std::vector<glm::vec3> &controlPoints, double duration, double startTime)
{
	auto currentTime = Time::FixedTime();
	auto elapsedTime = currentTime - startTime;
	float progress = static_cast<float>(elapsedTime / duration);
	progress = glm::clamp(progress, 0.0F, 1.0F);

	if (progress >= 1.0F)
	{
		// Animation is finished
		setPosition(controlPoints.back()); // Set the camera position to the last control point
		return true;
	}

	float segmentDuration = duration / (controlPoints.size() - 1);
	size_t currentSegment = static_cast<size_t>(progress * (controlPoints.size() - 1));
	float segmentProgress = static_cast<float>(elapsedTime - currentSegment * segmentDuration) / segmentDuration;

	glm::vec3 startPos = controlPoints[currentSegment];
	glm::vec3 endPos = controlPoints[currentSegment + 1];
	glm::vec3 newPos = glm::mix(startPos, endPos, segmentProgress);

	setPosition(newPos);

	return false;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, worldUp);
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getUp() const
{
	return up;
}

float Camera::getSpeed() const
{
	return speed;
}

void Camera::setPosition(glm::vec3 &value)
{
	position = value;
}

void Camera::setSpeed(const float value)
{
	speed = value;
}

void Camera::setSensitivity(const float value)
{
	sensitivity = value;
}

void Camera::setEye(const glm::vec3 &newEye)
{
	if (!isEyeLocked)
	{
		glm::vec3 newFront = glm::normalize(newEye);
		pitch = glm::degrees(glm::asin(newFront.y));
		yaw = glm::degrees(atan2(newFront.z, newFront.x));
		UpdateCameraVectors();
	}
}

//State
void Camera::ActivateInput()
{
	isInputActive = true;
}

void Camera::DeactivateInput()
{
	isInputActive = false;
}

bool Camera::isActiveInput() const 
{
	return isInputActive;
}

void Camera::LockEye()
{
	isEyeLocked = true;
}

void Camera::UnlockEye()
{
	isEyeLocked = false;
}

bool Camera::isLockedEye() const 
{
	return isEyeLocked;
}
