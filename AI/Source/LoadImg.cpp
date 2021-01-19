#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <GL\glew.h>
#include <stb_image.h>
#include "LoadImg.h"

GLuint LoadImg(const std::string& filePath){
	struct{
		std::string texPath = std::string();
		bool flipTex = true;
		int texTarget = GL_TEXTURE_2D;
		int texWrapParam = GL_REPEAT;
		int texFilterMin = GL_LINEAR_MIPMAP_LINEAR;
		int texFilterMag = GL_LINEAR;
	} params;

	params.texPath = filePath;

	unsigned int texRefID = 0;

	stbi_set_flip_vertically_on_load(params.flipTex); //OpenGL reads y/v tex coord in reverse so must flip tex vertically

	glGenTextures(1, &texRefID);
	int currTexRefID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currTexRefID);
	glBindTexture(params.texTarget, texRefID); //Make tex referenced by 'texRefIDs[i]' the tex currently bound to the currently active tex unit so subsequent tex commands will config it

	int width, height, colourChannelsAmt;
	unsigned char* data = stbi_load(params.texPath.c_str(), &width, &height, &colourChannelsAmt, 0);
	if(!data){
		(void)printf("Failed to load tex at \"%s\"\n", params.texPath.c_str());
		return 0;
	}
	glTexImage2D(params.texTarget, 0, colourChannelsAmt == 3 ? GL_RGB : GL_RGBA, width, height, 0, colourChannelsAmt == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data); //Free the img mem

	glGenerateMipmap(params.texTarget); //Gen required mipmap lvls for currently bound tex
	glTexParameteri(params.texTarget, GL_TEXTURE_WRAP_S, params.texWrapParam);
	glTexParameteri(params.texTarget, GL_TEXTURE_WRAP_T, params.texWrapParam);
	glTexParameteri(params.texTarget, GL_TEXTURE_MIN_FILTER, params.texFilterMin); //Nearest neighbour/Point filtering/interpolation when textures are scaled downwards
	glTexParameteri(params.texTarget, GL_TEXTURE_MAG_FILTER, params.texFilterMag); //Linear filtering/interpolation for upscaled textures
	glBindTexture(params.texTarget, currTexRefID); //Bind previously bound tex

	stbi_set_flip_vertically_on_load(false);

	return texRefID;
}