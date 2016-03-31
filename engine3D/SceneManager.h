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



		bool DeleteObj(const char* ID);
		void AddParent(const char* texPath, const char* objPath, const char* ID, glm::vec3 pos);
		void AddChild(const char* texPath, const char* objPath, const char* ID,const char* parent, glm::vec3 pos);

	private:

		Objects* CreateObj(const char* texPath, const char* objPath, glm::vec3 pos, const char* ID);
		void AddObj(Objects* obj);

		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;

		GLuint programID;
		GLuint MatrixID;
		GLuint ViewMatrixID;
		GLuint ModelMatrixID;

		GLuint TextureID;
		GLuint LightID;

		std::vector<Objects*>objects;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		GLFWwindow* window;

	};

	
}
