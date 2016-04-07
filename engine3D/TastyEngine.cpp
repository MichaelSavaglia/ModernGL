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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

	cubeObj = manager->LoadObjFile("wall.obj");
	suzanne = manager->LoadObjFile("suzanne.obj");
	mars = manager->LoadObjFile("mars.obj");

	uvmapID = manager->LoadTexture("uvmap.bmp");
	wallPaper = manager->LoadTexture("wallpaper.jpg");
	marsTex = manager->LoadTexture("Mars.bmp");

	firstObj = manager->CreateObj(suzanne, uvmapID, glm::vec3(0, 0, 0));
	cube = manager->CreateObj(cubeObj, wallPaper, glm::vec3(0, 0, 0));
	marsObj = manager->CreateObj(mars, marsTex, glm::vec3(0, 0, 5));

	manager->AddItemToRenderer(firstObj);
	manager->AddItemToRenderer(cube);
	manager->AddItemToRenderer(marsObj);


}

void TastyEngine::StartLoop()
{
	do{
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		manager->Update();
		manager->Render();


		//cube->SetPos(cube->GetPos() + glm::vec3(0.01, 0, 0));
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && mouseActive == false && qKeyUp)
		{
			qKeyUp = false;
			manager->ShowMouse();
			mouseActive = true;
			
		}
		else if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && mouseActive == true && qKeyUp)
		{
			qKeyUp = false;
			manager->HideMouse();
			mouseActive = false;

		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
		{
			qKeyUp = true;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwTerminate();
}