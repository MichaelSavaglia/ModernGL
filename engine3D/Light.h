#pragma once


#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>

class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 color, float power);
	~Light();

	void SetPos(glm::vec3 newPos);
	glm::vec3 GetPos();

	void SetColor(glm::vec3 newColor);
	glm::vec3 GetColor();

	void SetPower(float newPow);
	float GetPower();

private:
	glm::vec3 pos;
	glm::vec3 color;
	float power;
};

