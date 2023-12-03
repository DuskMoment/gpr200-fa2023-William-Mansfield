#include <stdio.h>
#include <math.h>
#include <iostream>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <wm/texture.h>
#include <wm/perlinNoise.h>
#include <wm/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

const int MAX_LIGHTS = 4;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);

ew::Camera camera;
ew::CameraController cameraController;

struct Light
{
	ew::Vec3 position; //world space
	ew::Vec3 color; //RGB
};

Light lights[MAX_LIGHTS];
int numberOfLights = 1;

bool orbit = false;


struct Material
{
	//all 1-0 ranges
	float ambientK = 0.1;
	float diffuseK = 0.3;
	float specular = 0.5;
	float rimK = 0.7;
	//not 1 - 0
	float shininess = 256.0;
	float rimAmbientIntestiy = 4;


};
Material material;

struct Wave { // adding struct for water wave
	float amplitude = 0.1;
	float wavelength = 10.0;
	float speed = 1.0;
	Material material;
};
Wave wave;

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

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// loading textures
	unsigned int mountainTexture = ew::loadTexture("assets/mountaintexture.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int waterTexture = ew::loadTexture("assets/watertexture.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int cellTexture = ew::loadTexture("assets/CellGrade.png", GL_CLAMP_TO_EDGE, GL_LINEAR);

	// shaders
	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader unlitShader("assets/unlit.vert", "assets/unlit.frag");
	ew::Shader waterShader("assets/water.vert", "assets/water.frag");

	// define unlit spehere mesh
	ew::Mesh unlitShpereMesh(ew::createSphere(0.2, 10));

	// define water mesh, transform, position
	ew::Mesh waterPlaneMesh(ew::createPlane(40.0f, 40.0f, 400)); // New water plane for water shaders
	ew::Transform waterPlaneTransform; // transform for water plan
	waterPlaneTransform.position = ew::Vec3(0.0, -1.0, 0); // setting pos for water plane transform
	wave.material.ambientK = 0.1;
	wave.material.diffuseK = 0.3;
	wave.material.specular = 0.5;
	wave.material.rimK = 0.7;
	wave.material.shininess = 256;
	wave.material.rimAmbientIntestiy = 4;


	ew::Transform unLitsphereTransfrom[MAX_LIGHTS];

	unLitsphereTransfrom[0].position = ew::Vec3(0.0, 1.5, -2.0);
	unLitsphereTransfrom[1].position = ew::Vec3(0.0, 1.5, 2.0);
	unLitsphereTransfrom[2].position = ew::Vec3(2.0, 1.5, 0.0);
	unLitsphereTransfrom[3].position = ew::Vec3(-2.0, 1.5, 0.0);

	// define land mesh, transform, position
	int seed = 300;
	ew::Mesh* landMesh = new ew::Mesh(wm::createLand(40.0f, 400, seed)); // last variable is seed for generation
	ew::Transform landTransform;
	landTransform.position = ew::Vec3(-20.0f, -6.0f, 20.0f);

	//default light positions and colors
	lights[0].position = unLitsphereTransfrom[0].position;
	lights[0].color = ew::Vec3(1.0, 1.0, 1.0);

	lights[1].position = unLitsphereTransfrom[1].position;
	lights[1].color = ew::Vec3(0.0, 1.0, 1.0);

	lights[2].position = unLitsphereTransfrom[2].position;
	lights[2].color = ew::Vec3(1.0, 0.0, 1.0);

	lights[3].position = unLitsphereTransfrom[3].position;
	lights[3].color = ew::Vec3(1.0, 1.0, 0.0);


	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader for mountains 
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mountainTexture);
		shader.setInt("_Texture", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cellTexture);
		shader.setInt("_CellTexture", 1);

		//shader model
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("cameraTarget", camera.target);

		//Draw land
		shader.setMat4("_Model", landTransform.getModelMatrix());
		landMesh->draw();
		
		// set positions and colors for lights
		for (int i = 0; i < numberOfLights; i++)
		{
			shader.setVec3("_Lights[" + std::to_string(i) + "].position", lights[i].position);
			shader.setVec3("_Lights[" + std::to_string(i) + "].color", lights[i].color);
		}

		//material uniforms 
		shader.setFloat("_Material.ambientK", material.ambientK);
		shader.setFloat("_Material.diffuseK", material.diffuseK);
		shader.setFloat("_Material.specular", material.specular);
		shader.setFloat("_Material.shininess", material.shininess);

		//added Rim lighting
		shader.setFloat("_Material.rimK", material.rimK);
		shader.setFloat("_Material.rimAmbientIntesity", material.rimAmbientIntestiy);

		//camera location
		shader.setVec3("cameraPos", camera.position);
		
		//nubmer of lights
		shader.setInt("numLights", numberOfLights);

		// unlit shader
		unlitShader.use();
		unlitShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		// draw unlit lights
		for (int i = 0; i < numberOfLights; i++)
		{
			unlitShader.setMat4("_Model", unLitsphereTransfrom[i].getModelMatrix());
			unlitShader.setVec3("_Color", lights[i].color);
			unlitShpereMesh.draw();
		}

		// water shader
		waterShader.use();
		waterShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		waterShader.setMat4("_Model", waterPlaneTransform.getModelMatrix());

		// water texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		waterShader.setInt("_Texture", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cellTexture);
		waterShader.setInt("_CellTexture", 1);

		// water material
		waterShader.setFloat("_Material.ambientK", wave.material.ambientK);
		waterShader.setFloat("_Material.diffuseK", wave.material.diffuseK);
		waterShader.setFloat("_Material.specular", wave.material.specular);
		waterShader.setFloat("_Material.shininess", wave.material.shininess);

		waterShader.setFloat("_Material.rimK", wave.material.rimK);
		waterShader.setFloat("_Material.rimAmbientIntesity", wave.material.rimAmbientIntestiy);

		// wave variables
		waterShader.setFloat("amplitude", wave.amplitude);
		waterShader.setFloat("wavelength", wave.wavelength);
		waterShader.setFloat("speed", wave.speed);
		waterShader.setInt("numWaves", 5.0f); // not sure if going to be needed
		waterShader.setFloat("time", time);
		waterShader.setVec2("direction", ew::Vec2(1.0f, 0.0f));

		waterShader.setInt("numLights", numberOfLights);

		for (int i = 0; i < numberOfLights; i++)
		{
			waterShader.setVec3("_Lights[" + std::to_string(i) + "].position", lights[i].position);
			waterShader.setVec3("_Lights[" + std::to_string(i) + "].color", lights[i].color);
		}

		waterShader.setVec3("cameraPos", camera.position);
		// draw water
		waterPlaneMesh.draw();

		// Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}
			if (ImGui::CollapsingHeader("material"))
			{
				ImGui::SliderFloat("ambient", &material.ambientK, 0, 1);
				ImGui::SliderFloat("diffuse", &material.diffuseK, 0, 1);
				ImGui::SliderFloat("specular", &material.specular, 0, 1);
				ImGui::SliderFloat("shininess", &material.shininess, 2, 256);
				ImGui::SliderFloat("ambient rim", &material.rimAmbientIntestiy, 0, 10);
				ImGui::SliderFloat("rim", &material.rimK, 0, 1);
				
			}
			ImGui::SliderInt("number of lights", &numberOfLights, 1, MAX_LIGHTS);

			ImGui::Checkbox("orbit", &orbit);
			if (orbit == true)
			{
				for (int i = 0; i < numberOfLights; i++)
				{
					ew::Vec3 nextPos = ew::Cross(ew::Vec3(0, 1, 0), lights[i].position);
					lights[i].position += nextPos * 0.001;
					unLitsphereTransfrom[i].position = lights[i].position;
				}
			}

			//lights UI
			for (int i = 0; i < numberOfLights; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("light"))
				{
					ImGui::ColorEdit3("light color", &lights[i].color.x);
					ImGui::DragFloat3("light position", &lights[i].position.x);
					unLitsphereTransfrom[i].position = lights[i].position;
				}
				ImGui::PopID();
			}
			//water IU
			if (ImGui::CollapsingHeader("water")) {
				ImGui::DragFloat("amplitude", &wave.amplitude, 0.05f);
				ImGui::DragFloat("wavelength", &wave.wavelength, 0.05f);
				ImGui::DragFloat("speed", &wave.speed, 0.05f);

				//water mat
				if (ImGui::CollapsingHeader("water material"))
				{
					ImGui::SliderFloat("ambient", &wave.material.ambientK, 0, 1);
					ImGui::SliderFloat("diffuse", &wave.material.diffuseK, 0, 1);
					ImGui::SliderFloat("specular", &wave.material.specular, 0, 1);
					ImGui::SliderFloat("shininess", &wave.material.shininess, 2, 256);
					ImGui::SliderFloat("ambient rim", &wave.material.rimAmbientIntestiy, 0, 10);
					ImGui::SliderFloat("rim", &wave.material.rimK, 0, 1);
				}
			}

			//land UI
			if (ImGui::CollapsingHeader("Land")) {
				if (ImGui::DragInt("seed", &seed))
				{
					delete landMesh;
					landMesh = new ew::Mesh(wm::createLand(40.0f, 400, seed));
				}
			}


			ImGui::ColorEdit3("BG color", &bgColor.x);
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
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}

