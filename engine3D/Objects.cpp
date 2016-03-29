#include "Objects.h"


Objects::Objects(GLuint indicesSize, GLuint vertexBuffer, GLuint UVBuffer, GLuint normalBuffer, GLuint elementBuffer, GLuint textureID, glm::vec3 pos, const char* ID)
{
	this->indicesSize = indicesSize;
	this->vertexBuffer = vertexBuffer;
	this->UVBuffer = UVBuffer;
	this->normalBuffer = normalBuffer;
	this->elementBuffer = elementBuffer;
	this->textureID = textureID;
	this->pos = pos;
	this->ID = ID;
}


Objects::~Objects()
{
}

