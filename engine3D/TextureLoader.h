#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glew.h>

#include <glfw3.h>

namespace loaders
{
	GLuint* loadBMP_custom(const char* path);
}