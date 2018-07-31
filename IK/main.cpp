#include <iostream>
#include "display.h"
#include "inputManager.h"
#include <Windows.h>
#include "vendors/imgui.h"
#include "vendors/imgui_impl_glfw_gl3.h"
#pragma comment(lib, "winmm.lib")

void renderGUI();

void init()
{
	GLCall(glfwSetKeyCallback(display.m_window,key_callback));
	GLCall(glfwSetWindowSizeCallback(display.m_window,window_size_callback));
	//ImGui_ImplGlfw_InstallCallbacks(menuWindow);
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

	PlaySound("./res/sound/Fantasy Game Loop.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	//inputManager input = inputManager();
	init();	
	//glfwSetInputMode(display.m_window,GLFW_STICKY_MOUSE_BUTTONS,1);
	
	/// Create GUI ///
	//GLFWwindow* menuWindow = glfwCreateWindow(1280, 720, "Snake Menu", NULL, NULL);
	ImGui::CreateContext();
	
	//glfwMakeContextCurrent(menuWindow);
	ImGui_ImplGlfwGL3_Init(display.m_window, false);
	//glfwSwapInterval(1); // Enable vsync
	// Setup style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();


	while(!glfwWindowShouldClose(display.m_window))
	{
		ImGui_ImplGlfwGL3_NewFrame();

		if (ikScn.movementActive) {
			Sleep(50);
			ikScn.makeIKChange();
		}
		
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		if (display.IsFullscreen())
		{
			GLint viewport[4];
			GLCall(glfwGetFramebufferSize(display.m_window, &viewport[2], &viewport[3]));
			window_size_callback(display.m_window, viewport[2], viewport[3]);
		}

		ikScn.draw(0,0,false); //change false to true for axis in every joint

		renderGUI();
		
		glfwPollEvents();
		display.SwapBuffers();
	}
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	return 0;
}

void renderGUI() {

	static float f = 0.0f;

	float oldF = f;
	//ikScn.shapeTransformation(ikScn.zCameraTranslate, (cameraPosition - glm::vec3(0, 0, 250)).z);
	if (ImGui::SliderFloat("Zoom", &f, -250.0f, 250.0f)) {
		ikScn.setPicked(-1);
		ikScn.shapeTransformation(ikScn.zCameraTranslate, f - oldF);
		/// Uncomment this if you want to save Sky View as latest Zoom
		//ikScn.cameraOriginalPosition = ikScn.GetCameras()[0]->getCameraPosition();
	}
	ImGui::Text("Points: %d", ikScn.pointsCounter);
	auto headPosition = ikScn.getBase(headLink);
	ImGui::Text("Position: x = %.2f, y = %.2f, z = %.2f", headPosition.x, headPosition.y, headPosition.z);
	auto destPosition = ikScn.getBase(ikScn.destinationIndex);
	ImGui::Text("Destination Position: x = %.2f, y = %.2f, z = %.2f", destPosition.x, destPosition.y, destPosition.z);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	if (ImGui::Button("Snake View")) {
		
		/*glm::vec3 camera = ikScn.GetCameras()[0]->getCameraPosition();
		glm::vec3 head = ikScn.getBase(headLink);
		auto change_x = head.x - camera.x;
		auto change_y = head.y - camera.y;
		auto change_z = head.z - camera.z;
		ikScn.setPicked(-1);
		ikScn.shapeTransformation(ikScn.xCameraTranslate, change_x);
		ikScn.shapeTransformation(ikScn.yCameraTranslate, change_y);
		ikScn.shapeTransformation(ikScn.zCameraTranslate, change_z);*/
		
		auto headPoint = ikScn.shapes[headLink]->getCenterOfRotation(glm::mat4(1));
		ikScn.GetCameras()[0]->setCameraPosition(glm::vec3(headPoint.x, headPoint.y, headPoint.z - 5));
		auto dir = glm::normalize(ikScn.getTipPosition(headLink) - ikScn.getBase(headLink));
		ikScn.GetCameras()[0]->setForward(glm::vec3(dir.x, dir.y, dir.z));
	}

	ImGui::SameLine();
	if (ImGui::Button("Sky View")) {
		auto headPoint = ikScn.shapes[headLink]->getCenterOfRotation(glm::mat4(1));
		ikScn.GetCameras()[0]->setCameraPosition(glm::vec3(headPoint.x, headPoint.y, headPoint.z - 50));
		ikScn.GetCameras()[0]->setForward(glm::vec3(0., 0.,  1.));
	}
	auto cameraPosition = ikScn.GetCameras()[0]->getCameraPosition();
	ImGui::Text("Camera Position: x = %.2f, y = %.2f, z = %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
