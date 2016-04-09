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
		LightColourID = glGetUniformLocation(programID, "LightColor");
		LightPowerID = glGetUniformLocation(programID, "LightPower");

		activeLight = new Light(glm::vec3(0, 10, 2), glm::vec3(1,1,1), 50);


	}


	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Update()
	{
		computeMatricesFromInputs(window, mouseActive);
		ProjectionMatrix = getProjectionMatrix();
		ViewMatrix = getViewMatrix();
	}


	void SceneManager::Render()
	{
		


		glm::vec3* tempLightColor = &activeLight->GetColor();
		float tempLightPower = activeLight->GetPower();
		glm::vec3* tempLightPos = &activeLight->GetPos();

		ImGui::Begin("Light data");
		
		if (ImGui::CollapsingHeader("Light color"))
		{
			ImGui::Text("Modify the rgb values\n" "of the lighting");
			ImGui::SetWindowSize("Light Color", ImVec2(150, 70));
			
			ImGui::SliderFloat("Red", &tempLightColor->x, 0.0f, 1.0f);
			ImGui::SliderFloat("Green", &tempLightColor->y, 0.0f, 1.0f);
			ImGui::SliderFloat("Blue", &tempLightColor->z, 0.0f, 1.0f);
		}
		if (ImGui::CollapsingHeader("Light position"))
		{
			ImGui::Text("Modify the rgb values\n" "of the lighting");

			ImGui::SliderFloat("x", &tempLightPos->x, -5.0f, 5.0f);
			ImGui::SliderFloat("y", &tempLightPos->y, -5.0f, 10.0f);
			ImGui::SliderFloat("z", &tempLightPos->z, -5.0f, 5.0f);
		}
		if (ImGui::CollapsingHeader("Light intensity"))
		{
			ImGui::SliderFloat("Intensity", &tempLightPower, 0.0f, 100.0f);
		}

		activeLight->SetColor(*tempLightColor);
		activeLight->SetPos(*tempLightPos);
		activeLight->SetPower(tempLightPower);
		
		ImGui::End();


		for (int i = 0; i < objects.size(); i++)
		{
			/*glm::quat MyQuaternion;
			glm::vec3 EulerAngles(90, 45, 0);
			MyQuaternion = glm::quat(EulerAngles);
			MyQuaternion =*/
			
			ModelMatrix = glm::mat4(1.0f);

			ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());
			
			
			
				if (objects[i]->GetRot() == true)
				{
					orientation = *objects[i]->GetRotVals();
					glm::mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
					ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());
					ModelMatrix *= rotationMatrix;
				}
			
			
			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

			ObjPack* tempObj = objects[i]->GetObjData();

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

			glBindTexture(GL_TEXTURE_2D, *objects[i]->GetTextureID());

			glUniform3f(LightColourID, tempLightColor->x, tempLightColor->y, tempLightColor->z);
			glUniform1f(LightPowerID, tempLightPower);
			glUniform3f(LightID, tempLightPos->x, tempLightPos->y, tempLightPos->z);

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

	ObjPack* SceneManager::LoadObjFile(const char* path)
	{
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

		return tempObjData;
	}

	GLuint* SceneManager::LoadTexture(const char* path)
	{
		Textures::Texture* tex = Textures::LoadIMG(path);
		return &tex->textureID;

	}


	void SceneManager::AddItemToRenderer(Objects* obj)
	{
		AddObj(obj);
	}


	void SceneManager::RemoveItemFromRenderer(Objects* obj)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (obj == objects[i])
			{
				objects.erase(objects.begin() + i);
			}
		}
	}

	Objects* SceneManager::CreateObj(ObjPack* objData, GLuint* texID, glm::vec3 pos)
	{
		Objects* tempObj = new Objects(objData, texID, pos);
		return tempObj;
	}



	void SceneManager::AddObj(Objects* obj)
	{
		objects.push_back(obj);
	}


	void SceneManager::ShowMouse()
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseActive = true;
	}

	void SceneManager::HideMouse()
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, width / 2, height / 2);
		mouseActive = false;
	}

	Light* SceneManager::CreateLight(glm::vec3 pos, glm::vec3 color, float power)
	{
		Light* tempLight = new Light(pos, color, power);
		return tempLight;
	}

	void SceneManager::AddLight(Light* light)
	{
		lights.push_back(light);
	}

	void SceneManager::UseLight(Light* light)
	{
		activeLight = light;
	}

	void SceneManager::DeleteLight(Light* light)
	{
		for (int i = 0; i < lights.size(); i++)
		{
			if (light == lights[i])
			{
				lights.erase(lights.begin() + i);
			}
		}
	}

	Objects* SceneManager::ClickObject(GLuint programID2)
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID2);

		pickingColorID = glGetUniformLocation(programID2, "PickingColor");
		pickingMatrixID = glGetUniformLocation(programID2, "MVP");

		glEnableVertexAttribArray(0);

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetID() != 0)
			{
				int ID = objects[i]->GetID();

				ModelMatrix = glm::mat4(1.0f);

				ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());

				if (objects[i]->GetRot() == true)
				{
					orientation = *objects[i]->GetRotVals();
					glm::mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
					ModelMatrix = glm::translate(ModelMatrix, objects[i]->GetPos());
					ModelMatrix *= rotationMatrix;
				}


				glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

				glUniformMatrix4fv(pickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
				 
				int r = (ID & 0x000000FF) >> 0;
				int g = (ID & 0x0000FF00) >> 8;
				int b = (ID & 0x00FF0000) >> 16;

				glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);


				ObjPack* tempObj = objects[i]->GetObjData();

				
				glBindBuffer(GL_ARRAY_BUFFER, tempObj->vertexBuffer);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempObj->elementBuffer);

				glDrawElements(GL_TRIANGLES, tempObj->indicesSize, GL_UNSIGNED_INT, 0);
				
				
			}
		}

		glDisableVertexAttribArray(0);

		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		double width, height;

		glfwGetCursorPos(window, &width, &height);

		glReadPixels(width, height, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;

		if (pickedID == 0x00ffffff)
			return false;
		else
		{
			for (int i = 0; i < objects.size(); i++)
			{
				if (pickedID == objects[i]->GetID())
				{
					return objects[i];
				}
			}
		}
	}
}

