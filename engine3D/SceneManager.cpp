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



			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

			glBindTexture(GL_TEXTURE_2D, objects[i]->GetTextureID());

			glm::vec3 lightPos = glm::vec3(4, 4, 4);
			glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

			glUniform1i(TextureID, 0);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetVertexBuffer());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetUVBuffer());
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetNormalBuffer());
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects[i]->GetElementBuffer());
			

			glDrawElements(GL_TRIANGLES, objects[i]->mesh.indices.size(), GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	Objects* SceneManager::CreateObj(const char* texPath, const char* objPath, glm::vec3 pos, const char* ID)
	{
		tinyobj::LoadObj(shapes, materials, err, objPath);
		Textures::Texture* tex = Textures::LoadIMG(texPath);

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
		

		Objects* obj = new Objects(shapes[0].mesh.indices.size(), vertBuf, uvBuf, normBuf, elemBuf, tex->textureID, pos, ID, tempMesh);

	}


	void SceneManager::AddParent(const char* texPath, const char* objPath, const char* ID, glm::vec3 pos)
	{
		Objects* tempObj = CreateObj(texPath, objPath, pos, ID);
		tempObj->SetParent(nullptr);
		AddObj(tempObj);
	}

	void SceneManager::AddChild(const char* texPath, const char* objPath, const char* ID, const char* parent, glm::vec3 pos)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (std::strcmp(parent, objects[i]->GetID()) == 0)
			{
				pos += objects[i]->GetPos();

			}
		}
		std::cout << "Error: Parent not found" << std::endl;
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

		return false;
	}
}