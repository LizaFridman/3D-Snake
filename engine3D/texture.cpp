#define GLEW_STATIC
#include "texture.h"
#include <GL\glew.h>
#include "stb_image.h"
#include <iostream>
#include "GLErrorHandler.h"

Texture::Texture(const std::string& fileName)
{
	int width, height, numComponents;
	//stbi_set_unpremultiply_on_load(1);
    unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);
	
	if (data == NULL) {
		std::cout << "Unable to load texture: " << fileName << std::endl;
	}
        
    GLCall(glGenTextures(1, &m_texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_texture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data)); 
	
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	Bind();

	if (data) {
		stbi_image_free(data);
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_texture));
}

void Texture::Bind()
{
	GLCall(glActiveTexture(GL_TEXTURE0));
	//GLCall(glBindTexture(GL_TEXTURE_2D, m_texture));
}

void Texture::Unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
