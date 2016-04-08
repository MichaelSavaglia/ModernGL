#include "Light.h"


Light::Light(glm::vec3 pos, glm::vec3 color, float power)
{
	this->pos = pos;
	this->color = color;
	this->power = power;
}


Light::~Light()
{
}

void Light::SetPos(glm::vec3 newPos)
{
	this->pos = newPos;
}

glm::vec3 Light::GetPos()
{
	return this->pos;
}

void Light::SetColor(glm::vec3 newColor)
{
	this->color = newColor;
}

glm::vec3 Light::GetColor()
{
	return this->color;
}

void Light::SetPower(float newPow)
{
	this->power = newPow;
}

float Light::GetPower()
{
	return this->power;
}