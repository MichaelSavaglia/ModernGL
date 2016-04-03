#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <glew.h>

#include <glfw3.h>
#include "Objects.h"
#include "ImageLoader.h"
#include "tiny_obj_loader.h"
#include "Camera.h"

namespace Manager
{
	class SceneManager
	{
	public:
		SceneManager(GLFWwindow* window, GLuint programID);
		~SceneManager();
		void Render();
		void Update();

		bool LoadObjFile(const char* path);


		bool DeleteObj(const char* ID);
		bool AddParent(const char* texPath, const char* objPath, const char* ID, glm::vec3 pos);
		bool AddChild(const char* texPath, const char* objPath, const char* ID,const char* parent, glm::vec3 pos);

		Objects* AccessObject(const char* ID);

	private:

		Objects* CreateObj(const char* texPath, const char* objPath, glm::vec3 pos, const char* ID);
		void AddObj(Objects* obj);
		Objects* FindObj(const char* ID);

		

		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;

		GLuint programID;
		GLuint MatrixID;
		GLuint ViewMatrixID;
		GLuint ModelMatrixID;

		GLuint TextureID;
		GLuint LightID;

		std::vector<Objects*>objects;
		std::vector<ObjPack*> objData;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		GLFWwindow* window;

	};

	
}
