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

		AmbientColorID = glGetUniformLocation(programID, "MaterialAmbientColorIn");
		SpecularColorID = glGetUniformLocation(programID, "MaterialSpecularColor");

		activeLight = new Light(glm::vec3(-0.5, 10.0, -0.5), glm::vec3(1,1,1), 30);
		this->programID = programID;
		glfwGetWindowSize(window, &width, &height);
		InitRenderTexture();

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
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);
		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(programID);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);
		glDepthMask(GL_TRUE);

		glm::vec3* tempLightColor = &activeLight->GetColor();
		float tempLightPower = activeLight->GetPower();
		glm::vec3* tempLightPos = &activeLight->GetPos();

		ImGui::Begin("Light options");
		{
			if (ImGui::CollapsingHeader("Light color"))
			{
				ImGui::Text("Modify the rgb values\n" "of the lighting");

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
				ImGui::Text("Modify the inensity\n" "of the lighting");
				ImGui::SliderFloat("Intensity", &tempLightPower, 0.0f, 100.0f);
			}
			

			activeLight->SetColor(*tempLightColor);
			activeLight->SetPos(*tempLightPos);
			activeLight->SetPower(tempLightPower);

			ImGui::End();
		}

		for (int i = 0; i < objects.size(); i++)
		{
			
			
			ModelMatrix = glm::mat4(1.0f);


			glm::mat4 translationMatrix = glm::translate(objects[i]->GetPos());

			orientation = objects[i]->GetRotVals();
			glm::mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);

			glm::mat4 scaleMatrix = glm::scale(objects[i]->GetScale());

			ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;


			glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

			ObjPack* tempObj = objects[i]->GetObjData();

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

			glBindTexture(GL_TEXTURE_2D, *objects[i]->GetActiveTextureID());

			glUniform3f(LightColourID, tempLightColor->x, tempLightColor->y, tempLightColor->z);
			glUniform1f(LightPowerID, tempLightPower);
			glUniform3f(LightID, tempLightPos->x, tempLightPos->y, tempLightPos->z);

			glm::vec3 tempAmbient = objects[i]->GetAmbient();
			glUniform3f(AmbientColorID, tempAmbient.x, tempAmbient.y, tempAmbient.z);
			glm::vec3 tempSpecular = objects[i]->GetSpecular();
			glUniform3f(SpecularColorID, tempSpecular.x, tempSpecular.y, tempSpecular.z);


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

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(quadProgamID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		glUniform1i(texID, 0);
		glUniform1f(timeID, (float)(glfwGetTime()*10.0f));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
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

	ObjPack* SceneManager::LoadObjCustom(const char* path)
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		loaders::loadOBJ(path, vertices, uvs, normals);

		std::vector<unsigned short> indices;
		std::vector<glm::vec3> indexed_vertices;
		std::vector<glm::vec2> indexed_uvs;
		std::vector<glm::vec3> indexed_normals;
		loaders::indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

		GLuint vertBuf;
		glGenBuffers(1, &vertBuf);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

		GLuint uvBuf;
		glGenBuffers(1, &uvBuf);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuf);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

		GLuint normBuf;
		glGenBuffers(1, &normBuf);
		glBindBuffer(GL_ARRAY_BUFFER, normBuf);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

		GLuint elemBuf;
		glGenBuffers(1, &elemBuf);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuf);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


		ObjPack* tempObjData = new ObjPack();
		tempObjData->elementBuffer = elemBuf;
		tempObjData->normalBuffer = normBuf;
		tempObjData->UVBuffer = uvBuf;
		tempObjData->vertexBuffer = vertBuf;
		tempObjData->indicesSize = indices.size();
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
		objects.push_back(obj);
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
		glUseProgram(programID2);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		pickingColorID = glGetUniformLocation(programID2, "PickingColor");
		pickingMatrixID = glGetUniformLocation(programID2, "MVP");

		glEnableVertexAttribArray(0);

		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->GetID() != 0)
			{
				int ID = objects[i]->GetID();

				ModelMatrix = glm::mat4(1.0f);

				glm::mat4 translationMatrix = glm::translate(objects[i]->GetPos());

				orientation = objects[i]->GetRotVals();
				glm::mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);

				glm::mat4 scaleMatrix = glm::scale(objects[i]->GetScale());

				ModelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

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
		return false;
		
	}

	bool SceneManager::InitRenderTexture()
	{
		frameBufferName = 0;
		glGenFramebuffers(1, &frameBufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferName);

		glGenTextures(1, &renderedTexture);

		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glGenRenderbuffers(1, &depthRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
		drawBuffers[0] = GL_COLOR_ATTACHMENT0;

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			return false;

		const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
		};

		glGenBuffers(1, &quad_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

		quadProgamID = loaders::LoadShaders("PassThroughVert.glsl", "WobblyFrag.glsl");
		texID = glGetUniformLocation(quadProgamID, "renderedTexture");
		timeID = glGetUniformLocation(quadProgamID, "time");
	}

	void SceneManager::TestCel(GLuint programID1, GLuint programID2)
	{

		glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);
		glDepthMask(GL_TRUE);
		glUseProgram(programID1);

		matrixID = glGetUniformLocation(programID1, "MVP");
		colorID = glGetUniformLocation(programID1, "u_color1");
		offsetID = glGetUniformLocation(programID1, "offset");

		ModelMatrix = glm::mat4(1.0f);


		ModelMatrix = glm::translate(ModelMatrix, objects[4]->GetPos());

		
		orientation = objects[2]->GetRotVals();
		glm::mat4 rotationMatrix = eulerAngleYXZ(orientation.y, orientation.x, orientation.z);
		ModelMatrix = glm::translate(ModelMatrix, objects[2]->GetPos());
		ModelMatrix *= rotationMatrix;
		

		glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

		

		ObjPack* tempObj = objects[2]->GetObjData();
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, tempObj->vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);



		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, tempObj->normalBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempObj->elementBuffer);

		
		/* Chris - look here!*/
		
		//glm::vec3 color(0, 0, 0);
		glm::vec3 color(1, 1, 1);
		glUniform4f(colorID, color.x, color.y, color.z, 1.0f);
		glUniform1f(offsetID, 0.65f);
		glDrawElements(GL_TRIANGLES, tempObj->indicesSize, GL_UNSIGNED_INT, 0);

		glCullFace(GL_CW);
		glDepthMask(GL_FALSE);
		//glm::vec3 color2(1, 1, 1);
		glm::vec3 color2(1, 1, 1);
		glUniform4f(colorID, color2.x, color2.y, color2.z, 1.0f);
		glUniform1f(offsetID, 0.0f);
		glDrawElements(GL_TRIANGLES, tempObj->indicesSize, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	bool SceneManager::AddGamestate(const char* state)
	{	 
		for (int i = 0; i < gameStates.size(); i++)
		{
			if (strcmp(gameStates[i], state) == 0)
			{
				std::cout << "Error: Gamestate already exists! " << state << std::endl;
				return false;
			}
		}
		gameStates.push_back(state);
		return true;
	}	 
		 
	bool SceneManager::DeleteGameState(const char* state)
	{	 
		for (int i = 0; i < gameStates.size(); i++)
		{
			if (strcmp(gameStates[i], state) == 0)
			{
				gameStates.erase(gameStates.begin() + i);
				return true;
			}
		}
		std::cout << "Error: Gamestate doesn't exists! " << state << std::endl;
		return false;
	}	 
		 
	bool SceneManager::SetActiveState(const char* state)
	{
		for (int i = 0; i < gameStates.size(); i++)
		{
			if (strcmp(gameStates[i], state) == 0)
			{
				activeState = state;
				return true;
			}
		}
		std::cout << "Error: Gamestate doesn't exists! " << state << std::endl;
		return false;
	}

	const char* SceneManager::GetCurrentState()
	{
		return activeState;
	}

	void SceneManager::ClearRenderer()
	{
		objects.clear();
	}
}

