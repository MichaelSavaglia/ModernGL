#include "Objects.h"


Objects::Objects(ObjPack* objData, GLuint textureID, glm::vec3 pos, const char* ID)
{
	this->objData = objData;
	this->textureID = textureID;
	this->pos = pos;
	this->ID = ID;

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
