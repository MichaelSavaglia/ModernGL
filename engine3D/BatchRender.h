#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <glew.h>

#include <glfw3.h>
#include "Objects.h"
#include "ImageLoader.h"

namespace Renderer
{
	class BatchRender
	{
	public:
		BatchRender();
		~BatchRender();
		void Render();
		void AddObj(const char* texPath, const char* objPath);

		private:
		std::vector<Objects*>objects;
	};

	
}
