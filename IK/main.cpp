#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include "vendors/imgui.h"
#include "vendors/imgui_impl_glfw_gl3.h"

void init()
{
	GLCall(glfwSetKeyCallback(display.m_window,key_callback));
	GLCall(glfwSetWindowSizeCallback(display.m_window,window_size_callback));
}

int main(int argc, char** argv)
{
	
	Vertex vertices[] =
	{
		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
		Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

		Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
		Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

		Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
		Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
	};



	unsigned int indices[] = {0, 1, 2,
							  0, 2, 3,

							  6, 5, 4,
							  7, 6, 4,

							  10, 9, 8,
							  11, 10, 8,

							  12, 13, 14,
							  12, 14, 15,

							  16, 17, 18,
							  16, 18, 19,

							  22, 21, 20,
							  23, 22, 20
	                          };


	ikScn.init(vertices,indices,sizeof(vertices)/sizeof(vertices[0]), sizeof(indices)/sizeof(indices[0]));
	ikScn.addShader("./res/shaders/basicShader");
	ikScn.addShader("./res/shaders/pickingShader");	
	//scn.addShape("./res/monkey3.obj","./res/grass.bmp");
	//Shader shader("./res/basicShader");

	//inputManager input = inputManager();
	init();	
	//glfwSetInputMode(display.m_window,GLFW_STICKY_MOUSE_BUTTONS,1);
	
	/// Create GUI ///
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(display.m_window, true);
	// Setup style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();


	while(!glfwWindowShouldClose(display.m_window))
	{
		ImGui_ImplGlfwGL3_NewFrame();

		if (ikScn.movementActive) {
			Sleep(30);
			ikScn.makeIKChange();
		}
		
			display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
			if(display.IsFullscreen())
			{
				GLint viewport[4];
				GLCall(glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3] ));
				window_size_callback(display.m_window, viewport[2],viewport[3]);
			}
		ikScn.draw(0,0,false); //change false to true for axis in every joint
		glfwPollEvents();

		auto cameraPosition = ikScn.getTipPosition(-1);
		static float f = 0.0f;
		ikScn.setPicked(-1);
		float oldF = f;
		//ikScn.shapeTransformation(ikScn.zCameraTranslate, (cameraPosition - glm::vec3(0, 0, 250)).z);
		if (ImGui::SliderFloat("Zoom", &f, -100.0f, 100.0f)) {
				ikScn.shapeTransformation(ikScn.zCameraTranslate, f - oldF);
		}
		ImGui::Text("Points: %d", ikScn.pointsCounter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		display.SwapBuffers();
		
		
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}
