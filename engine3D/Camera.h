#pragma once

#include <glfw3.h>


// Include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
using namespace glm;



void computeMatricesFromInputs(GLFWwindow* window, bool active);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
