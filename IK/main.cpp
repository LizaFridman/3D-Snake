#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include "./res/vendors/imgui.h"
#include "./res/vendors/imgui_impl_glfw_gl3.h"
#pragma comment(lib, "winmm.lib")

void renderGUI();

Vertex boxVertices[] =
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



unsigned int boxIndices[] = { 0, 1, 2,
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

void init()
{
	GLCall(glfwSetKeyCallback(display.m_window,key_callback));
	GLCall(glfwSetWindowSizeCallback(display.m_window,window_size_callback));
	//ImGui_ImplGlfw_InstallCallbacks(menuWindow);
}

int main(int argc, char** argv)
{
	ikScn.init(boxVertices, boxIndices, sizeof(boxVertices) / sizeof(boxVertices[0]), sizeof(boxIndices) / sizeof(boxIndices[0]));
	ikScn.addShader("./res/shaders/basicShader");
	ikScn.addShader("./res/shaders/pickingShader");
	init();
	PlaySound("./res/sound/Fantasy Game Loop.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	/// Create GUI ///
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(display.m_window, false);
	//glfwSwapInterval(1); // Enable vsync
	ImGui::StyleColorsClassic();

	while(!glfwWindowShouldClose(display.m_window))
	{
		ImGui_ImplGlfwGL3_NewFrame();

		if (ikScn.gameMode == ONGOING) {
			Sleep(50);
			ikScn.changeScene();
			ikScn.updatePositions(0.1f);
		}
		/*
		auto shape = ikScn.is_snake_collided();
		if (shape != NULL) {
			auto type = shape->type;
			switch (type) {
				case OBSTICLE_BOX:
					ikScn.gameOver = true;
					break;
				case INVISIBLE:
				case SNAKE_LINK:
				default:
					break;
			}

			std::cout << "Skane collided with Shape Type = " << type << std::endl;
		}*/

		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		if (display.IsFullscreen())
		{
			GLint viewport[4];
			GLCall(glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3]));
			window_size_callback(display.m_window, viewport[2], viewport[3]);
		}
    
		ikScn.draw(0,0,false);

		renderGUI();
		
		glfwPollEvents();
		display.SwapBuffers();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

void renderGUI() {

	ImGui::Text("Points: %d", ikScn.pointsCounter);
	
	if (ImGui::Button("Pause Game")) {
		ikScn.pausePressed();
	}
	
	static float f = 0.0f;
	float oldF = f;
	if (ImGui::SliderFloat("Zoom", &f, -250.0f, 250.0f)) {
		ikScn.setPicked(-1);
		ikScn.shapeTransformation(ikScn.zCameraTranslate, f - oldF);
		/// Uncomment this if you want to save Sky View as latest Zoom
		//ikScn.cameraOriginalPosition = ikScn.GetCameras()[0]->getCameraPosition();
	} 
	static glm::vec4 color(20, 5., 0, 1.);
	ImGui::ColorEdit3("Scene Color", (float*)&ikScn.sceneColor, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_RGB); // Edit 3 floats representing a color
	//ikScn.sceneColor = glm::vec3(color);

	auto cameraPosition = ikScn.GetCameras()[ikScn.viewIndex]->getCameraPosition();
	ImGui::Text("Camera Position: x = %.2f, y = %.2f, z = %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	auto headPosition = ikScn.getBase(headLink);
	ImGui::Text("Position: x = %.2f, y = %.2f, z = %.2f", headPosition.x, headPosition.y, headPosition.z);
	auto destPosition = ikScn.destinationPosition;
	ImGui::Text("Destination Position: x = %.2f, y = %.2f, z = %.2f", destPosition.x, destPosition.y, destPosition.z);

	if (ImGui::Button("Snake View")) {
		ikScn.viewIndex = snakeViewIndex;
	}
	ImGui::SameLine();
	if (ImGui::Button("Sky View")) {
		/*auto headPoint = ikScn.shapes[headLink]->getCenterOfRotation(glm::mat4(1));
		ikScn.GetCameras()[0]->setCameraPosition(glm::vec3(headPoint.x, headPoint.y, headPoint.z - 50));
		ikScn.GetCameras()[0]->setForward(glm::vec3(0., 0.,  1.));*/
		ikScn.viewIndex = skyViewIndex;
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
