#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>

#include <wm/camera.h>
#include <wm/transformations.h>
#include <wm/shader.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, wm::Camera* camera, wm::CameraControls* controls, float &deltaTime);


//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
wm::Transform cubeTransforms[NUM_CUBES];
wm::Camera camera;
wm::CameraControls controls;
//bool orbit;
float speed = 0.01;


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	wm::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}
	//setting starting vals
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0, 0, 0);
	camera.fov = 60;
	camera.orthoSize = 6;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	camera.aspectRatio = static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT;

	float prevTime = 0;
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;
		moveCamera(window, &camera, &controls, deltaTime);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

	
		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", camera.ProjectionMarix() * camera.VeiwMatrix() *cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			//ImGui::SliderFloat("orbitSpeed", &speed, 0.001, 0.09);
			//ImGui::Checkbox("Orbit", &orbit);
			//if (orbit)
			//{
			//	
			//	ew::Vec3 nextPos = ew::Normalize(ew::Cross(ew::Vec3(0, 1, 0),camera.position));
			//	//why does this work
			//	camera.position += nextPos * speed;
			//	
			//}
			ImGui::DragFloat3("Position", &camera.position.x, 0.01f);
			ImGui::DragFloat3("Target", &camera.target.x, 0.01f);
			ImGui::DragFloat("FOV", &camera.fov, 0.01f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			ImGui::DragFloat("Orhto hight", &camera.orthoSize, 0.01f);
			ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.01f);
			ImGui::DragFloat("Far Plane", &camera.farPlane, 0.01f);
			ImGui::Text("fly camera");
			ImGui::DragFloat("Speed", &controls.moveSpeed, 0.1f);
			ImGui::Text("Pitch = %f", controls.pitch);
			ImGui::Text("Yaw = %f", controls.yaw);
			if (ImGui::Button("reset"))
			{
				controls.pitch = 0;
				controls.yaw = 0;
				camera.position = ew::Vec3(0, 0, 5);
				camera.target = ew::Vec3(0, 0, 0);
				
			}
	
		
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.aspectRatio = static_cast<float> (width)/ height;
}

void moveCamera(GLFWwindow* window, wm::Camera* camera, wm::CameraControls* controls, float &deltaTime)
{
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//get mouse position on the screen
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (controls->firstMouse)
	{
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}


	//gets delta and add to yaw and pitch
	controls->yaw += (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	controls->pitch -= (mouseY - controls->prevMouseY) * controls->mouseSensitivity;

	if (controls->pitch > 89)
	{
		controls->pitch = 89;
	}
	else if (controls->pitch < -89)
	{
		controls->pitch = -89;
	}

	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	float radYaw = ew::Radians(controls->yaw);
	float radPitch= ew::Radians(controls-> pitch);

	//create our forward
	ew::Vec3 forward = ew::Vec3((sin(radYaw) * cos(radPitch)), sin(radPitch), (-cos(radYaw) * cos(radPitch)));
	camera->target = camera->position + forward;

	//keyboard movment
	ew::Vec3 right = ew::Normalize(ew::Cross(forward, ew::Vec3(0, 1, 0)));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A))
	{
		camera->position -= right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D))
	{
		camera->position += right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E))
	{
		camera->position += up * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q))
	{
		camera->position -= up * controls->moveSpeed * deltaTime;
	}
	camera->target = camera->position + forward;


};
