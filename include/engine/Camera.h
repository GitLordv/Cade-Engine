#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "engine/shared/Types.h"


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

	Camera(CameraConfig &config);

	void KeyboardInput(CameraDirection direction, double deltaTime);
	void MouseMovingInput(float xoffset, float yoffset, bool constrainPitch = true);

	void setPosition(glm::vec3 &value);
	void setSpeed(float &value);
	void setSensitivity(float &value);

	glm::mat4 getViewMatrix() const;
	glm::vec3 getPosition() const;

	//Animate
	bool AnimatePos(glm::vec3 &start, const glm::vec3 end, double duration, double startTime);


private:

	void UpdateCameraVectors();

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
