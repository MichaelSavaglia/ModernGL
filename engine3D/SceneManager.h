#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>

#include <glew.h>

#include <glfw3.h>
#include <gtc\quaternion.hpp>
#include <gtx\quaternion.hpp>
#include <gtx/euler_angles.hpp>

#include "Objects.h"
#include "ImageLoader.h"
#include "tiny_obj_loader.h"
#include "Camera.h"
#include "Light.h"
#include "imgui\imgui.h"


namespace Manager
{
	class SceneManager
	{
	public:
		SceneManager(GLFWwindow* window, GLuint programID);
		~SceneManager();
		void Render();
		void Update();


		ObjPack* LoadObjFile(const char* path);
		GLuint* LoadTexture(const char* path);

		Objects* CreateObj(ObjPack* objData, GLuint* texID, glm::vec3 pos);

		Light* CreateLight(glm::vec3 pos, glm::vec3, float power);
		void AddLight(Light* light);
		void UseLight(Light* light);
		void DeleteLight(Light* light);

		void AddItemToRenderer(Objects* obj);
		void RemoveItemFromRenderer(Objects* obj);
		void ShowMouse();
		void HideMouse();

		Objects* ClickObject(GLuint programID2);

	private:
		vec3 orientation = vec3(0, 0, 0);

		void AddObj(Objects* obj);

		bool mouseActive = false;

		glm::mat4 ModelMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;

		GLuint programID;
		GLuint MatrixID;
		GLuint ViewMatrixID;
		GLuint ModelMatrixID;

		GLuint TextureID;
		GLuint LightID;
		GLuint LightColourID;
		GLuint LightPowerID;

		Light* activeLight;

		std::vector<Light*>lights;

		std::vector<Objects*>objects;
		std::vector<ObjPack*> objData;

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		GLFWwindow* window;

	};

	
}
