#include <GL/glew.h>
#include "GLErrorHandler.h"


void GLClearError()
{
	//std::cout << "Error Handler Here!" << std::endl;
	while (glGetError() != GL_NO_ERROR);
}

bool GLCheckError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] (" << error << "): " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}



