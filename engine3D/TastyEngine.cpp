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
	silhouetteID = loaders::LoadShaders("SilhouetteVertex.glsl", "SilhouetteFrag.glsl");

	glUseProgram(programID);
	
	manager = new Manager::SceneManager(window, programID);

	wallsObj = manager->LoadObjFile("objects/newWalls.obj");




	floorObj = manager->LoadObjFile("objects/floor.obj");
	floorTexID = manager->LoadTexture("textures/woodFloor.jpg");
	floor = manager->CreateObj(floorObj, floorTexID, glm::vec3(0, 0, 0.18));
	manager->AddItemToRenderer(floor);

	//platformObj = manager->LoadObjFile("platform.obj");
	platformTexID = manager->LoadTexture("textures/brickwall.jpg");
	wallPaper = manager->LoadTexture("textures/plaster.jpg");
	platform = manager->CreateObj(floorObj, wallPaper, glm::vec3(0, 15, 0));
	manager->AddItemToRenderer(platform);
	

	uvmapID = manager->LoadTexture("textures/uvmap.bmp");

	cubeObj = manager->LoadObjFile("objects/cube.obj");
	cube = manager->CreateObj(cubeObj, uvmapID, glm::vec3(-7.2, 4.3, 0));
	manager->AddItemToRenderer(cube);

	/* Mars and all textures*/
	marsObj = manager->LoadObjFile("objects/mars.obj");
	marsTex = manager->LoadTexture("textures/Mars.bmp");
	mars = manager->CreateObj(marsObj, marsTex, glm::vec3(-0.4, 4.3, 0));
	manager->AddItemToRenderer(mars);
	mars->SetID(1);
	GLuint* europa = manager->LoadTexture("textures/europaColor.png");
	GLuint* mercury = manager->LoadTexture("textures/mercuryColor.png");
	GLuint* uranus = manager->LoadTexture("textures/uranusColor.png");
	mars->AddTexture(europa);
	mars->AddTexture(mercury);
	mars->AddTexture(uranus);
	/* Mars and all textures*/

	spaceShipObj = manager->LoadObjFile("objects/spaceShip.obj");
	spaceShipTex = manager->LoadTexture("textures/spaceShip.png");
	spaceShip = manager->CreateObj(spaceShipObj, spaceShipTex, glm::vec3(3, 4.4, 0));
	spaceShip->SetScale(glm::vec3(0.08, 0.08, 0.08));
	spaceShip->SetID(10);
	manager->AddItemToRenderer(spaceShip);


	suzanne = manager->LoadObjFile("objects/suzanne.obj");
	firstObj = manager->CreateObj(suzanne, uvmapID, glm::vec3(-3.8, 4.3, 0));


	walls = manager->CreateObj(wallsObj, platformTexID, glm::vec3(0, 0, 0));

	
	firstObj->AddTexture(europa);


	walls->SetRotate(glm::vec3(0, 90, 0));
	manager->AddItemToRenderer(firstObj);
	manager->AddItemToRenderer(walls);
	

	
	firstObj->SetID(2);
	cube->SetID(3);

	selectedObject = nullptr;
}

void TastyEngine::StartLoop()
{
	do{
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		manager->Update();

		ImGui_ImplGlfwGL3_NewFrame();


		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		
	
		if (selectedObject != nullptr)
		{
			glm::vec3 tempPos = selectedObject->GetPos();
			glm::vec3 tempAmbient = selectedObject->GetAmbient();
			glm::vec3 tempSpecular = selectedObject->GetSpecular();
			glm::vec3 tempScale = selectedObject->GetScale();
			glm::vec3 tempRotation = selectedObject->GetRotVals();

			ImGui::Begin("Selected object data");
			ImGui::SetWindowSize(ImVec2(300, 400));
			if (ImGui::CollapsingHeader("Object Position"))
			{
				ImGui::SliderFloat("X", &tempPos.x, -8.0f, 8.0f);
				ImGui::SliderFloat("Y", &tempPos.y, 3.0f, 10.0f);
				ImGui::SliderFloat("Z", &tempPos.z, -4.0f, 4.0f);
			}
			if (ImGui::CollapsingHeader("Object Scale"))
			{
				ImGui::SliderFloat("x", &tempScale.x, 0.0f, 4.0f);
				ImGui::SliderFloat("y", &tempScale.y, 0.0f, 4.0f);
				ImGui::SliderFloat("z", &tempScale.z, 0.0f, 4.0f);
			}
			if (ImGui::CollapsingHeader("Object Rotation"))
			{
				ImGui::SliderFloat("x", &tempRotation.x, 0.0f, 360.0f);
				ImGui::SliderFloat("y", &tempRotation.y, 0.0f, 360.0f);
				ImGui::SliderFloat("z", &tempRotation.z, 0.0f, 360.0f);
			}
			if (ImGui::CollapsingHeader("Object Ambient"))
			{
				ImGui::SliderFloat("R", &tempAmbient.x, 0.0f, 1.0f);
				ImGui::SliderFloat("G", &tempAmbient.y, 0.0f, 1.0f);
				ImGui::SliderFloat("B", &tempAmbient.z, 0.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("Object Specular"))
			{
				ImGui::SliderFloat("RED", &tempSpecular.x, 0.0f, 1.0f);
				ImGui::SliderFloat("GREEN", &tempSpecular.y, 0.0f, 1.0f);
				ImGui::SliderFloat("BLUE", &tempSpecular.z, 0.0f, 1.0f);
			}
			if (ImGui::CollapsingHeader("Object Textures"))
			{
				for (int i = 0; i < selectedObject->CheckNumTextures(); i++)
				{
					std::string textureNum = "Texture ";
					textureNum += std::to_string(i + 1);
					
					if (ImGui::Button(textureNum.c_str())) selectedObject->SetActiveTexture(i + 1);
				}
			}
			ImGui::End();

			selectedObject->SetPos(tempPos);
			selectedObject->SetAmbient(tempAmbient);
			selectedObject->SetSpecular(tempSpecular);
			selectedObject->SetScale(tempScale);
			selectedObject->SetRotate(tempRotation);
		}


		mars->SetRotate(glm::vec3(0, monkeyRot, 0));
		firstObj->SetRotate(glm::vec3(0, monkeyRot, 0));
		monkeyRot += 0.2;

		
		InputHandling();

		
		
		//manager->TestCel(silhouetteID, 2);
		manager->Render();
		
		
		
		ImGui::Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	glfwTerminate();
}

void TastyEngine::InputHandling()
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && mouseActive == false && qKeyUp)
	{
		qKeyUp = false;
		manager->ShowMouse();
		mouseActive = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && mouseActive == true && qKeyUp)
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
}

