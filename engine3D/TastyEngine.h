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
#include "vboIndexer.h"
#include "SceneManager.h"

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

	GLuint VertexArrayID;

	Textures::Texture* texture;

	ObjPack* cubeObj;
	ObjPack* suzanne;
	ObjPack* mars;
	GLuint* uvmapID;
	GLuint* wallPaper;
	GLuint* marsTex;
	Objects* firstObj;
	Objects* cube;
	Objects* marsObj;

	bool mouseActive = false;
	bool qKeyUp = true;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	
	Manager::SceneManager* manager;

	bool moved = false;
};

