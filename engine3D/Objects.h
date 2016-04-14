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


	GLuint* GetActiveTextureID(){ return activeTextureID; }
	void SetActiveTexture(int index){ activeTextureID = textures[index - 1]; }

	void AddTexture(GLuint* textureID){ textures.push_back(textureID); }
	int CheckNumTextures(){ return textures.size(); }

	int GetID(){ return ID; }
	void SetID(int ID){ this->ID = ID; }
	glm::vec3 GetPos(){ return pos; }
	void SetPos(glm::vec3 nPos){ this->pos = nPos; }

	glm::vec3 GetSpecular(){ return specularColor; }
	void SetSpecular(glm::vec3 newSpec){ specularColor = newSpec; }

	glm::vec3 GetAmbient(){ return ambientColor; }
	void SetAmbient(glm::vec3 newAmbi){ ambientColor = newAmbi; }

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

	GLuint* activeTextureID;
	std::vector<GLuint*> textures;

	glm::vec3 pos;
	int ID;

	glm::vec3 ambientColor;
	glm::vec3 specularColor;


};



