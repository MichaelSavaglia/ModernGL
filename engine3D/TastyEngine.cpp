#include "TastyEngine.h"


TastyEngine::TastyEngine()
{
	Init();
	LoadObjects();
	StartLoop();
}


TastyEngine::~TastyEngine()
{
}

bool TastyEngine::Init()
{

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	window = glfwCreateWindow(1024, 768, "Pretty Triangle v2.0", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);



	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialise GLEW.\n");
		return false;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void TastyEngine::LoadObjects()
{
	/*glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loaders::loadOBJ("suzanne.obj", vertices, uvs, normals);

	
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	loaders::indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);


	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);*/

	programID = loaders::LoadShaders("VertexShader.glsl", "FragmentShader.glsl");

	/*MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");

	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");*/

	texture = Textures::LoadIMG("uvmap.bmp");

	glUseProgram(programID);

	manager = new Manager::SceneManager(window, programID);

	manager->AddObj("uvmap.bmp", "suzanne.obj", glm::vec3(0.0f, 0.0f, 1.0f), "FirstOBJ");
}

void TastyEngine::StartLoop()
{
	do{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		manager->Update();
		manager->Render();
		//computeMatricesFromInputs(window);
		/*glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();*/


		///////////////////////////////////////////////////////////////////////////
		/*if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-0.1f, 0.0f, 0.0f));
		}*/
		///////////////////////////////////////////////////////////////////////////
		//glm::mat4 mvp = ProjectionMatrix * ViewMatrix * ModelMatrix;



		
		//glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);


		/*glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glUniform1i(TextureID, 0);*/
		

		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
			);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glVertexAttribPointer(
			2,                               
			3,                               
			GL_FLOAT,                        
			GL_FALSE,                        
			0,                               
			(void*)0                         
			);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

		glBindTexture(GL_TEXTURE_2D, 1);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
		glDisableVertexAttribArray(0);*/

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwTerminate();
}