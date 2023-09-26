#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <wm/shader.h>

struct Vertex {
	float x, y, z;
	float u, v;
};
unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

Vertex vertices[4] = {
	//x   //y  //z //u //v
	{-1.0, -1.0, 0.0 , 0.0, 0.0}, // bottom L 0
	{ 1.0,-1.0, 0.0, 1.0, 0.0}, // bottom R 1
	{1.0, 1.0, 0.0, 1.0, 1.0}, //top R 2
	{-1.0, 1.0, 0.0, 0.0, 1.0} //top L 3
 
};

unsigned int indices[6] = {
	0,1,2,
	0,2,3
};
//const char* vertexShaderSource = R"(
//	#version 450
//	layout(location = 0) in vec3 vPos;
//	void main(){
//		gl_Position = vec4(vPos,1.0);
//	}
//)";

//const char* fragmentShaderSource = R"(
//	#version 450
//	out vec4 FragColor;
//	uniform vec3 _Color;
//	uniform float _Brightness;
//	void main(){
//		FragColor = vec4(_Color * _Brightness,1.0);
//	}
//)";

float sunTopColor[3] = {0.8, 0.6, 0.0 };
float sunBottomColor[3] = { 0.6, 0.0, 0.0 };
float speed = 1.0;
float sunRadius = 0.0; 

float fgColor[3] = { 0.2, 0.2, 0.2 };

float bgTopColor[3] = { 0.1, 0.1, 0.2 };
float bgBottomColor[3] = { 1.0, 0.5, 0.3 };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	
	wm::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	
	unsigned int vao = createVAO(vertices, 4 ,indices, 2);
	
	glBindVertexArray(vao);

	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
	
		shader.setFloat("_Time", (float)glfwGetTime());
		shader.setFloat("_Speed", speed);
		shader.setVec2("_Resolution", SCREEN_WIDTH, SCREEN_HEIGHT);

		//backround 
		shader.setVec3("_BgTopColor",bgTopColor[0], bgTopColor[1],bgTopColor[2]);
		shader.setVec3("_BgBottomColor", bgBottomColor[0], bgBottomColor[1], bgBottomColor[2]);

		//sun 
		shader.setVec3("_SunTopColor", sunTopColor[0], sunTopColor[1], sunTopColor[2]);
		shader.setVec3("_SunBottomColor", sunBottomColor[0], sunBottomColor[1], sunBottomColor[2]);
		shader.setFloat("_SunRad", sunRadius);

		//forground
		shader.setVec3("_FgColor", fgColor[0], fgColor[1], fgColor[2]);
		
		

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			//changed
			ImGui::ColorEdit3("sunTopColor", sunTopColor);
			ImGui::ColorEdit3("sunBottomColor", sunBottomColor);
			ImGui::SliderFloat("sunRadius", &sunRadius, -1.0f, 1.0f);
			ImGui::SliderFloat("Speed", &speed, 0.0f, 2.0f);
			//unchanged
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			//changed
			ImGui::ColorEdit3("forgroundColor", fgColor);
			ImGui::ColorEdit3("backroundTopColor", bgTopColor);
			ImGui::ColorEdit3("backRoundBottomColor", bgBottomColor);
			//put new UI here
			//add sun speed and add radius
			
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


unsigned int createShader(GLenum shaderType, const char* sourceCode) {
	//Create a new vertex shader object
	unsigned int shader = glCreateShader(shaderType);
	//Supply the shader object with source code
	glShaderSource(shader, 1, &sourceCode, NULL);
	//Compile the shader object
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}
	return shader;
}

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 5, vertexData, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenVertexArrays(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices * 3, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);


	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

