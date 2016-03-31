#pragma once

#include <iostream>

#include <glew.h>

#include <glfw3.h>

#include <glm.hpp>
#include "tiny_obj_loader.h"

class Objects
{
public:
	Objects(GLuint indicesSize, GLuint vertexBuffer, GLuint UVBuffer, GLuint normalBuffer, GLuint elementBuffer, GLuint textureID, glm::vec3 pos, const char* ID, tinyobj::mesh_t mesh);
	~Objects();


	GLuint GetVertexBuffer() {return vertexBuffer;};
	GLuint GetUVBuffer(){ return UVBuffer; };
	GLuint GetNormalBuffer(){ return normalBuffer; };
	GLuint GetTextureID(){ return textureID; };
	GLuint GetElementBuffer(){ return elementBuffer; }
	GLuint GetIndicesSize(){ return indicesSize; }
	const char* GetID(){ return ID; }
	glm::vec3 GetPos(){ return pos; }
	tinyobj::mesh_t mesh;

	void SetParent(Objects* parent);
	void AddChild(Objects* child);


private:
	GLuint vertexBuffer;
	GLuint UVBuffer;
	GLuint normalBuffer;
	GLuint textureID;
	GLuint elementBuffer;
	GLuint indicesSize;


	Objects* parent;
	std::vector<Objects*> children;


	glm::vec3 pos;
	const char* ID;
};

