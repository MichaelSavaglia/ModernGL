#include "BatchRender.h"

namespace Renderer
{

	BatchRender::BatchRender()
	{

	}


	BatchRender::~BatchRender()
	{
	}

	void BatchRender::Render()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetVertexBuffer());
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetUVBuffer());
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, objects[i]->GetNormalBuffer());
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindTexture(GL_TEXTURE_2D, objects[i]->GetTextureID());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			glDrawArrays(GL_TRIANGLES, 0, objects[i]->GetVertexCount());

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	void AddObj(const char* texPath, const char* objPath)
	{
		//Tinyobjloader load objpath

		Textures::LoadIMG(texPath);
	}
}