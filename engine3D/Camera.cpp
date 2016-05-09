#include "Camera.h"



glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::vec3 position = glm::vec3(-0.966785431, 6.8, 6.85130072);

float horizontalAngle = 3.141f;

float verticalAngle = 0.0484695360f;

float initialFoV = 45.0f;

float speed = 4.0f;
float mouseSpeed = 0.05f;


void computeMatricesFromInputs(GLFWwindow* window, bool active)
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	
	double xpos, ypos;
	if (active == false)
	{
		glfwGetCursorPos(window, &xpos, &ypos);

		glfwSetCursorPos(window, width / 2, height / 2);

	
		horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * deltaTime * float(height / 2 - ypos);
		if (verticalAngle < -1.2)
			verticalAngle = -1.2;
		if (verticalAngle > 1.2)
			verticalAngle = 1.2;
	}

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));

	glm::vec3 right(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f));

	glm::vec3 up = glm::cross(right, direction);
	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position -= direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position -= right * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += right * deltaTime * speed;

	ProjectionMatrix	= glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);

	position.y = 6.8;
	ViewMatrix = glm::lookAt(
		position,
		position + direction,
		up);

	lastTime = currentTime;
}
