#pragma once

#include <glew.h>

#include <glfw3.h>

#include <glm.hpp>

class Objects
{
public:
	Objects(GLuint indicesSize, GLuint vertexBuffer, GLuint UVBuffer, GLuint normalBuffer, GLuint elementBuffer, GLuint textureID, glm::vec3 pos, const char* ID);
	~Objects();


	GLuint GetVertexBuffer() {return vertexBuffer;};
	GLuint GetUVBuffer(){ return UVBuffer; };
	GLuint GetNormalBuffer(){ return normalBuffer; };
	GLuint GetTextureID(){ return textureID; };
	GLuint GetElementBuffer(){ return elementBuffer; }
	GLuint GetIndicesSize(){ return indicesSize; }
	glm::vec3 GetPos(){ return pos; }

private:
	GLuint vertexBuffer;
	GLuint UVBuffer;
	GLuint normalBuffer;
	GLuint textureID;
	GLuint elementBuffer;
	GLuint indicesSize;

	glm::vec3 pos;
	const char* ID;
};

