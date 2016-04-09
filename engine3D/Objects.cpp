#include "Objects.h"


Objects::Objects(ObjPack* objData, GLuint* texID, glm::vec3 pos)
{
	this->objData = objData;
	this->textureID = texID;
	this->pos = pos;
	ambientColor = glm::vec3(0.1, 0.1, 0.1);
	specularColor = glm::vec3(0.1, 0.1, 0.1);
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

void Objects::SetRotate(glm::vec3 rot)
{
	rotate = true;
	rot = glm::vec3(glm::radians((float)rot.x), glm::radians((float)rot.y), glm::radians((float)rot.z));
	this->rot = rot;

}

bool Objects::GetRot()
{
	return rotate;
}

glm::vec3* Objects::GetRotVals()
{
	return &rot;
}
