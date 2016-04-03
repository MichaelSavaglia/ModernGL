#include "SceneManager.h"

namespace Manager
{

	SceneManager::SceneManager(GLFWwindow* window, GLuint programID)
	{
		this->window = window;

		MatrixID = glGetUniformLocation(programID, "MVP");
		ViewMatrixID = glGetUniformLocation(programID, "V");
		ModelMatrixID = glGetUniformLocation(programID, "M");

		TextureID = glGetUniformLocation(programID, "myTextureSampler");

		LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	}


	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Update()
	{
		
		computeMatricesFromInputs(window);
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
	}


	void SceneManager::Render()
	{

		for (int i = 0; i < objects.size(); i++)
		{

			glm::mat4 ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());
			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

			ObjPack* tempObj = objects[i]->GetObjData();

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

			glBindTexture(GL_TEXTURE_2D, objects[i]->GetTextureID());

			glm::vec3 lightPos = glm::vec3(4, 4, 4);
			glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

			glUniform1i(TextureID, 0);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, tempObj->vertexBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, tempObj->UVBuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, tempObj->normalBuffer);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempObj->elementBuffer);
			

			glDrawElements(GL_TRIANGLES, tempObj->indicesSize, GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	bool SceneManager::LoadObjFile(const char* path)
	{
		for (int i = 0; i < objData.size(); i++)
		{
			if (std::strcmp(path, objData[i]->ID) == 0)
			{
				std::cout << "Error: Obj already loaded" << std::endl;
				return false;
			}
		}


		tinyobj::LoadObj(shapes, materials, err, path);

		tinyobj::mesh_t tempMesh = shapes[0].mesh;

		GLuint vertBuf;
		glGenBuffers(1, &vertBuf);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
		glBufferData(GL_ARRAY_BUFFER, tempMesh.positions.size() * sizeof(tempMesh.positions[0]), &tempMesh.positions[0], GL_STATIC_DRAW);

		GLuint uvBuf;
		glGenBuffers(1, &uvBuf);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuf);
		glBufferData(GL_ARRAY_BUFFER, tempMesh.texcoords.size() * sizeof(tempMesh.texcoords[0]), &tempMesh.texcoords[0], GL_STATIC_DRAW);

		GLuint normBuf;
		glGenBuffers(1, &normBuf);
		glBindBuffer(GL_ARRAY_BUFFER, normBuf);
		glBufferData(GL_ARRAY_BUFFER, tempMesh.normals.size() * sizeof(tempMesh.normals[0]), &tempMesh.normals[0], GL_STATIC_DRAW);

		GLuint elemBuf;
		glGenBuffers(1, &elemBuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, tempMesh.indices.size()*sizeof(tempMesh.indices[0]), &tempMesh.indices[0], GL_STATIC_DRAW);


		ObjPack* tempObjData = new ObjPack();
		tempObjData->elementBuffer = elemBuf;
		tempObjData->normalBuffer = normBuf;
		tempObjData->UVBuffer = uvBuf;
		tempObjData->vertexBuffer = vertBuf;
		tempObjData->indicesSize = tempMesh.indices.size();
		tempObjData->ID = path;

		objData.push_back(tempObjData);
	}

	Objects* SceneManager::CreateObj(const char* texPath, const char* objID, glm::vec3 pos, const char* ID)
	{
		
		Textures::Texture* tex = Textures::LoadIMG(texPath);

		for (int i = 0; i < objData.size(); i++)
		{
			if (std::strcmp(objID, objData[i]->ID) == 0)
			{
				Objects* obj = new Objects(objData[i], tex->textureID, pos, ID);
				return obj;
			}
			
		}
		return false;
	}


	bool SceneManager::AddParent(const char* texPath, const char* objID, const char* ID, glm::vec3 pos)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (std::strcmp(ID, objects[i]->GetID()) == 0)
			{
				std::cout << "Error: An object with this ID already exists: " << ID << std::endl;
				return false;
			}
		}
		Objects* tempObj = CreateObj(texPath, objID, pos, ID);
		tempObj->SetParent(nullptr);
		AddObj(tempObj);
		return true;
	}

	bool SceneManager::AddChild(const char* texPath, const char* objPath, const char* ID, const char* parent, glm::vec3 pos)
	{
		if (FindObj(ID) != false)
		{
				std::cout << "Error: An object with this ID already exists: " << ID << std::endl;
				return false;
				
		}
		

		for (int i = 0; i < objects.size(); i++)
		{
			if (std::strcmp(parent, objects[i]->GetID()) == 0)
			{
				pos += objects[i]->GetPos();
				Objects* tempObj = CreateObj(texPath, objPath, pos, ID);
				tempObj->SetParent(objects[i]);
				AddObj(tempObj);
				
				return true;
			}
		}
		std::cout << "Error: Parent not found" << std::endl;
		return false;
	}

	void SceneManager::AddObj(Objects* obj)
	{
		objects.push_back(obj);
	}

	bool SceneManager::DeleteObj(const char* ID)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (std::strcmp(ID, objects[i]->GetID()) == 0)
			{
				objects.erase(objects.begin() + i);
				return true;
			}
		}
		std::cout << "Error: Object not found: " << ID << std::endl;
		return false;
	}

	Objects* SceneManager::AccessObject(const char* ID)
	{
		Objects* tempObj = FindObj(ID);
		if (tempObj == NULL)
		{
			std::cout << "Error: Object not found: " << ID << std::endl;
			return false;
		}
		return tempObj;
	}

	Objects* SceneManager::FindObj(const char* ID)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (std::strcmp(ID, objects[i]->GetID()) == 0)
			{
				return objects[i];
			}
		}
		return false;
	}
}