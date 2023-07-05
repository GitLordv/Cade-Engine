#pragma once

#include "engine/shared/Utils.h"


//Default camera options
static constexpr GLfloat YAW = -90.0f;
static constexpr GLfloat PITCH = 0.0f;
static constexpr GLfloat SPEED = 2.5f;
static constexpr GLfloat SENSITIVITY = 0.1f;
static constexpr GLfloat ZOOM = 45.0f;

struct CameraConfig
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat pitch = 0.0f;
	GLfloat yaw = -90.0f;
};

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};


class Camera
{
public:

	Camera(CameraConfig &config);
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	void KeyboardInput(CameraDirection direction, float deltaTime);
	void MouseMovingInput(float xoffset, float yoffset, bool constrainPitch);
	void MouseScrollingInput(float yoffset);
	
	glm::mat4 GetViewMatrix();


private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	float Zoom;
	float MovingSpeed;
	float MouseSensitivity;


	void UpdateCameraVectors();
};

