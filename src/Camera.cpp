#include "engine/Camera.h"


Camera::Camera(CameraConfig &config) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovingSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = config.position;
	WorldUp = config.up;
	Yaw = config.yaw;
	Pitch = config.pitch;
	UpdateCameraVectors();
}

Camera::Camera(glm::vec3 position = glm::vec3(0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovingSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovingSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}


void Camera::KeyboardInput(CameraDirection direction, float deltaTime)
{
	float velocity = MovingSpeed * deltaTime;
	
	if (direction == CameraDirection::FORWARD)
	{
		Position += Front * velocity;
	}
	if (direction == CameraDirection::BACKWARD)
	{
		Position -= Front * velocity;
	}
	if (direction == CameraDirection::LEFT)
	{
		Position -= Right * velocity;
	}
	if (direction == CameraDirection::RIGHT)
	{
		Position += Right * velocity;
	}
}


void Camera::MouseMovingInput(float xoffset, float yoffset, bool constrainPitch = true)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch)
	{
		if (Pitch > 89.0f)
		{
			Pitch = 89.0f;
		}
		if (Pitch < -89.0f)
		{
			Pitch = -89.0f;
		}
	}

	UpdateCameraVectors();
}


void Camera::MouseScrollingInput(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
	{
		Zoom -= yoffset;
	}
	if (Zoom <= 1.0f)
	{
		Zoom = 1.0f;
	}
	if (Zoom >= 45.0f)
	{
		Zoom = 45.0f;
	}
}


void Camera::UpdateCameraVectors()
{
	glm::vec3 front{0.0f};
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}


glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}
