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
#include "Light.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"



using namespace glm;


class TastyEngine
{
public:
	TastyEngine();
	~TastyEngine();

	bool Init();
	void LoadObjects();
	void StartLoop();

	void InputHandling();


private:
	GLFWwindow* window;

	GLuint programID;
	GLuint pickingID;
	GLuint silhouetteID;

	GLuint VertexArrayID;

	Textures::Texture* texture;

	ObjPack* wallsObj;
	ObjPack* suzanne;
	ObjPack* marsObj;

	ObjPack* cubeObj;
	Objects* cube;

	ObjPack* floorObj;
	GLuint* floorTexID;
	Objects* floor;

	ObjPack* platformObj;
	GLuint* platformTexID;
	Objects* platform;


	GLuint* uvmapID;
	GLuint* wallPaper;
	GLuint* marsTex;

	Objects* firstObj;
	float monkeyRot = 0;


	Objects* walls;
	Objects* mars;

	Objects* selectedObject;

	int lastTime;
	int nbFrames;
	


	bool mouseActive = false;
	bool qKeyUp = true;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	
	Manager::SceneManager* manager;

	bool moved = false;
};

