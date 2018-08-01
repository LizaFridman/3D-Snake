#pragma once   //maybe should be static class
#include "GLFW\glfw3.h"
#include "IK.h"

const int DISPLAY_WIDTH = 1200;
const int DISPLAY_HEIGHT = 800;
const float FAR = 500.0f;
const float NEAR = 1.0f;
const float CAM_ANGLE = 60.0f;
float relation = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;

Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

IK ikScn(glm::vec3(0.0f, 5.0f, -20.0f), CAM_ANGLE, relation, NEAR, FAR);

float factor = 1.0;

double x1 = 0, x2=0;
double ys1 = 0, y2 = 0;
float depth;
	
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			GLCall(glfwSetWindowShouldClose(window, GLFW_TRUE));
			break;
		case GLFW_KEY_RIGHT:
			/// Go to the right of the screen
			std::cout<< "right: "<< std::endl;
			ikScn.setDirectionRight();
			//ikScn.UpdateSnakeMovement();
			break;
		case GLFW_KEY_LEFT:
			/// Go to the left of the screen
			std::cout<< "left: "<< std::endl;
			ikScn.setDirectionLeft();
			//ikScn.UpdateSnakeMovement();
			break;
		case GLFW_KEY_UP:
			/// Go to the up of the screen
			std::cout << "up: " << std::endl;
			ikScn.setDirectionUp();
			//ikScn.UpdateSnakeMovement();
			break;
		case GLFW_KEY_DOWN:
			/// Go to the down of the screen
			std::cout<< "down: "<< std::endl;
			ikScn.setDirectionDown();
			//ikScn.UpdateSnakeMovement();
			break;
		case GLFW_KEY_D:
			ikScn.setPicked(0);
			ikScn.shapeTransformation(ikScn.xGlobalTranslate, -1.f);
			break;
		case GLFW_KEY_A:
			ikScn.setPicked(0);
			ikScn.shapeTransformation(ikScn.xGlobalTranslate, 1.f);
			break;
		case GLFW_KEY_W:
			ikScn.setPicked(0);
			ikScn.shapeTransformation(ikScn.zGlobalTranslate, 1.f);
			break;
		case GLFW_KEY_S:
			ikScn.setPicked(0);
			ikScn.shapeTransformation(ikScn.zGlobalTranslate, -1.f);
			break;
		default:
			break;
		}
		//ikScn.UpdateSnakeMovement();
	}
	else {
		ikScn.destinationPosition = glm::vec3(0, 0, 100);
		ikScn.movementActive = true;
	}
}
	
	void updatePosition(double xpos, double ypos)
	{
		x1  = x2;
		x2 = xpos;
		ys1 = y2; 
		y2 = ypos;
	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		
		ikScn.resize(width,height,NEAR,FAR);
		relation = (float)width/(float)height;
	}