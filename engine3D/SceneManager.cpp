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

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glUniform1i(TextureID, 0);

		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	}


	void SceneManager::Render()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());
			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;


			glBindTexture(GL_TEXTURE_2D, objects[i]->GetTextureID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);


			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetVertexBuffer());
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetUVBuffer());
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetNormalBuffer());
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects[i]->GetElementBuffer());
			


			
			glDrawElements(GL_TRIANGLES, objects[i]->GetIndicesSize(), GL_UNSIGNED_INT, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	void SceneManager::AddObj(const char* texPath, const char* objPath, glm::vec3 pos, const char* ID)
	{
		tinyobj::LoadObj(shapes, materials, err, objPath);
		Textures::Texture* tex = Textures::LoadIMG(texPath);

		GLuint vertBuf;
		glGenBuffers(1, &vertBuf);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
		glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.positions.size() * sizeof(float), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);

		GLuint uvBuf;
		glGenBuffers(1, &uvBuf);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuf);
		glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.texcoords.size() * sizeof(float), &shapes[0].mesh.texcoords[0], GL_STATIC_DRAW);

		GLuint normBuf;
		glGenBuffers(1, &normBuf);
		glBindBuffer(GL_ARRAY_BUFFER, normBuf);
		glBufferData(GL_ARRAY_BUFFER, shapes[0].mesh.normals.size() * sizeof(float), &shapes[0].mesh.normals[0], GL_STATIC_DRAW);

		GLuint elemBuf;
		glGenBuffers(1, &elemBuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[0].mesh.indices.size()*sizeof(unsigned int), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);
		

		Objects* obj = new Objects(shapes[0].mesh.indices.size(), vertBuf, uvBuf, normBuf, elemBuf, tex->textureID, pos, ID);

		objects.push_back(obj);
	}
}