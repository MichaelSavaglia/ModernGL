#pragma once

#include <iostream>

#include <glew.h>

#include <glfw3.h>

#include <glm.hpp>
#include "tiny_obj_loader.h"

struct ObjPack
{
	const char* ID; 
	GLuint vertexBuffer;
	GLuint UVBuffer;
	GLuint normalBuffer;
	GLuint elementBuffer;
	GLuint indicesSize;
};

class Objects
{
public:
	Objects(ObjPack* objData, GLuint* texID, glm::vec3 pos);
	~Objects();


	ObjPack* GetObjData(){ return objData; }
	GLuint* GetTextureID(){ return textureID; }
	const char* GetID(){ return ID; }
	glm::vec3 GetPos(){ return pos; }
	void SetPos(glm::vec3 nPos){ this->pos = nPos; }

	void SetParent(Objects* parent);
	void AddChild(Objects* child);

	void SetRotate(glm::vec3 rot);
	bool GetRot();
	glm::vec3* GetRotVals();

	//virtual void Update();

private:

	glm::vec3 rot;
	bool rotate = false;
	ObjPack* objData;

	Objects* parent;
	std::vector<Objects*> children;

	GLuint* textureID;
	glm::vec3 pos;
	const char* ID;

protected:

};



