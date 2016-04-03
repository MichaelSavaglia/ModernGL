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
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	programID = loaders::LoadShaders("VertexShader.glsl", "FragmentShader.glsl");

	glUseProgram(programID);
	
	manager = new Manager::SceneManager(window, programID);

	manager->LoadObjFile("cube.obj");
	manager->LoadObjFile("suzanne.obj");
	manager->LoadObjFile("mars.obj");

	manager->AddParent("uvmap.bmp", "suzanne.obj", "FirstOBJ", glm::vec3(0.0f, 0.0f, 1.0f));
	manager->AddParent("uvmap.bmp", "suzanne.obj", "FirstOBJ 2", glm::vec3(0.0f, 5.0f, 0.0f));
	manager->AddParent("uvmap.bmp", "cube.obj", "cube", glm::vec3(10.0f, 1.0f, 0.0f));
	manager->AddChild("uvmap.bmp", "mars.obj", "firstChild", "cube", glm::vec3(3.0f, 1.0f, 0.0f));
	manager->AddChild("uvmap.bmp", "mars.obj", "mars", "firstChild", glm::vec3(-1.0f, -1.0f, 0.0f));
	
}

void TastyEngine::StartLoop()
{
	do{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		manager->Update();
		manager->Render();


		
		

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwTerminate();
}