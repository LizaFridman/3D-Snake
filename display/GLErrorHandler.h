//#ifndef GLERRORHANDLER_INCLUDED_H
//#define GLERRORHANDLER_INCLUDED_H
#include <iostream>
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
x;\
ASSERT(GLCheckError(#x, __FILE__, __LINE__))

/*class GLErrorHandler {

public:
*/
	 void GLClearError();
	 bool GLCheckError(const char* function, const char* file, int line);

//};
//#endif