#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <SOIL.h>

#include "LoadShader.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "ImageLoader.h"

using namespace glm;


class TastyEngine
{
public:
	TastyEngine();
	~TastyEngine();

	bool Init();
	void LoadObjects();
	void StartLoop();

private:
	GLFWwindow* window;
	GLuint programID;
	GLuint Texture;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint VertexArrayID;
	GLuint normalBuffer;
	Textures::Texture* texture;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;

	GLuint TextureID;
	GLuint LightID;

	std::vector<glm::vec3> vertices;
};

