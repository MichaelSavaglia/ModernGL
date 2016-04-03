#include "Objects.h"


Objects::Objects(ObjPack* objData, GLuint* texID, glm::vec3 pos)
{
	this->objData = objData;
	this->textureID = texID;
	this->pos = pos;


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
