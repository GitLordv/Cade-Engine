#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "engine/TriggerBox.h"
#include <vector>


struct CameraConfig
{
	glm::vec3 position{0.0F, 0.0F, 5.0F};
	float speed = 0.95F;
	float sens = 0.09F;
};

enum class CameraDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


class Camera
{
public:

	Camera();
	Camera(const CameraConfig &config);

	void KeyboardInput(const CameraDirection direction, double deltaTime);
	void MouseMovingInput(float xoffset, float yoffset, bool constrainPitch = true);

	void setPosition(glm::vec3 &value);
	void setSpeed(const float value);
	void setSensitivity(const float value);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;
	glm::vec3 getUp() const;
	float getSpeed() const;

	//Animate
	bool AnimatePos(glm::vec3 &start, const glm::vec3 end, double duration, double startTime);
	bool AnimatePath(std::vector<glm::vec3> &controlPoints, double duration, double startTime);

	//State
	void ActivateInput();
	void DeactivateInput();
	bool isActiveInput();

private:

	void UpdateCameraVectors();
	void PreInitCamera(const CameraConfig &config);

	bool inputActive;

	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;
	float speed;
	float sensitivity;
};
