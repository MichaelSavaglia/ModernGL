#include "Objects.h"


Objects::Objects(GLuint indicesSize, GLuint vertexBuffer, GLuint UVBuffer, GLuint normalBuffer, GLuint elementBuffer, GLuint textureID, glm::vec3 pos, const char* ID, tinyobj::mesh_t mesh)
{
	this->indicesSize = indicesSize;
	this->vertexBuffer = vertexBuffer;
	this->UVBuffer = UVBuffer;
	this->normalBuffer = normalBuffer;
	this->elementBuffer = elementBuffer;
	this->textureID = textureID;
	this->pos = pos;
	this->ID = ID;
	this->mesh = mesh;
}


Objects::~Objects()
{
}

void Objects::SetParent(Objects* parent)
{
	if (this->parent == nullptr)
	{
		this->parent = parent;
	}
	else
	{
		std::cout << "Error: This object already has a parent" << std::endl;
	}
}

void Objects::AddChild(Objects* child)
{
	children.push_back(child);
}
