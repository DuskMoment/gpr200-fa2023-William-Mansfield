#pragma once
#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"


namespace wm
{
	unsigned int loadTexture(const char* filePath, int wrapMode, int minFilterMode, int maxFiliterMode)
	{

		stbi_set_flip_vertically_on_load(true);

		int width, hight, numComponents;

		unsigned char* data = stbi_load(filePath, &width, &hight, &numComponents, 0);

		if (data == NULL)
		{
			printf("Failed to load image %s", filePath);
			return 0;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		unsigned int format;
		switch (numComponents)
		{
		case 1: format = GL_RED;
			break;
		case 2: format = GL_RG;
			break;
		case 3: format = GL_RGB;
			break;
		case 4: format = GL_RGBA;
			break;
		default: format = GL_RGBA;
			break;
		}
		//ask for this 
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, hight, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		//min filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
		//mag filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFiliterMode);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);

		return texture;
	}

}
	



