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


	window = glfwCreateWindow(1280, 720, "Pretty Triangle v2.0", NULL, NULL);
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

	ImGui_ImplGlfwGL3_Init(window, true);


	return true;
}

void TastyEngine::LoadObjects()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	programID = loaders::LoadShaders("VertexShader.glsl", "FragmentShader.glsl");
	pickingID = loaders::LoadShaders("PickVertex.glsl", "PickFrag.glsl");

	glUseProgram(programID);
	
	manager = new Manager::SceneManager(window, programID);

	cubeObj = manager->LoadObjFile("newWalls.obj");
	suzanne = manager->LoadObjFile("suzanne.obj");
	mars = manager->LoadObjFile("mars.obj");


	floorObj = manager->LoadObjFile("floor.obj");
	floorTexID = manager->LoadTexture("woodFloor.jpg");
	floor = manager->CreateObj(floorObj, floorTexID, glm::vec3(0, 0, 0.18));
	manager->AddItemToRenderer(floor);

	//platformObj = manager->LoadObjFile("platform.obj");
	platformTexID = manager->LoadTexture("platformtex.jpg");
	wallPaper = manager->LoadTexture("plaster.jpg");
	platform = manager->CreateObj(floorObj, wallPaper, glm::vec3(0, 15, 0));
	manager->AddItemToRenderer(platform);
	

	uvmapID = manager->LoadTexture("uvmap.bmp");

	marsTex = manager->LoadTexture("Mars.bmp");

	firstObj = manager->CreateObj(suzanne, uvmapID, glm::vec3(-2.1, 3, 0));
	cube = manager->CreateObj(cubeObj, platformTexID, glm::vec3(0, 0, 0));
	marsObj = manager->CreateObj(mars, marsTex, glm::vec3(0.01, 3, 0));
	
	cube->SetRotate(glm::vec3(0, 90, 0));
	manager->AddItemToRenderer(firstObj);
	manager->AddItemToRenderer(cube);
	manager->AddItemToRenderer(marsObj);
	firstObj->SetID(2);
	marsObj->SetID(1);

	selectedObject = nullptr;
}

void TastyEngine::StartLoop()
{
	ImVec4 clear_color = ImColor(114, 144, 154);
	do{
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		manager->Update();

		ImGui_ImplGlfwGL3_NewFrame();
	
	
		if (selectedObject != nullptr)
		{
			glm::vec3 tempPos = selectedObject->GetPos();
			glm::vec3 tempAmbient = selectedObject->GetAmbient();
			glm::vec3 tempSpecular = selectedObject->GetSpecular();
			ImGui::Begin("Selected object data");
			ImGui::SetWindowSize(ImVec2(300, 400));
			if (ImGui::CollapsingHeader("Object Position"))
			{
				ImGui::SliderFloat("X", &tempPos.x, -5.0f, 5.0f);
				ImGui::SliderFloat("Y", &tempPos.y, -5.0f, 5.0f);
				ImGui::SliderFloat("Z", &tempPos.z, -5.0f, 5.0f);
			}
			if (ImGui::CollapsingHeader("Object Ambient"))
			{
				ImGui::SliderFloat("R", &tempAmbient.x, 0.0f, 1.0f);
				ImGui::SliderFloat("G", &tempAmbient.y, 0.0f, 1.0f);
				ImGui::SliderFloat("B", &tempAmbient.z, 0.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("Object Specular"))
			{
				ImGui::SliderFloat("R", &tempSpecular.x, 0.0f, 1.0f);
				ImGui::SliderFloat("G", &tempSpecular.y, 0.0f, 1.0f);
				ImGui::SliderFloat("B", &tempSpecular.z, 0.0f, 1.0f);
			}
			ImGui::End();

			selectedObject->SetPos(tempPos);
			selectedObject->SetAmbient(tempAmbient);
			selectedObject->SetSpecular(tempSpecular);
		}


		marsObj->SetRotate(glm::vec3(0, monkeyRot, 0));
		firstObj->SetRotate(glm::vec3(0, monkeyRot, 0));
		monkeyRot += 0.2;

		
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

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && mouseActive == false)
		{
			selectedObject = manager->ClickObject(pickingID);
		}

		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		manager->Render();
		ImGui::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwTerminate();
}