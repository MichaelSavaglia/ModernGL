#include "Camera.h"



glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::vec3 position = glm::vec3(0, 3, 5);

float horizontalAngle = 3.141f;

float verticalAngle = 0.0f;

float initialFoV = 45.0f;

float speed = 3.0f;
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
	

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		position += direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		position -= direction * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		position -= right * deltaTime * speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		position += right * deltaTime * speed;

	ProjectionMatrix	= glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);

	ViewMatrix = glm::lookAt(
		position,
		position + direction,
		up);

	lastTime = currentTime;
}
