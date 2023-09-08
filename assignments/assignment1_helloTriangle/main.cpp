#include <stdio.h>
#include <math.h>
//gald imprortant to do open gl calls 
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

//this will make our triangle
float vertices[21] =
{
	// all Z's zero becuase it is a 2d object
	//x   //y   //z   //r  //g  //b // a
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0,//bottem left
	0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, // bottem right
	0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 1.0 // top left

};
//vertex shader glsl is the shader language that is why it is in quotes
//has to set gl_Position and needs to be vec4 goes from model to clip space
const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	layout(location = 1) in vec4 vColor;
	out vec4 Color;
	uniform float _Time;
	void main(){
		Color = vColor;
		vec3 offset = vec3(tan(cos(vPos.y+_Time)),abs(sin(vPos.x + _Time)),0)*0.5;
		gl_Position = vec4(vPos + offset,1.0);
	}
)";
//fragemnt shader
//handles the color
// 1.0 is white and 0 is black
// vect4(r,g,b,alpha(trasparency))
const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	in vec4 Color;
	uniform float _Time;
	
	void main(){
		FragColor = Color * abs(sin(_Time));
	}
)";
unsigned int createVAO(float* vertexData, int numVertices)
{

	//needs to return vao
	//Vertex Array Object (VAO)
	unsigned int vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Vertex Buffer Object (VBO) (used to send data to the GPU a buffer)
	//define new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	//after creating our new vbo buffer we then put it in the GL_ARRAY_BUFFER slot making it our new buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allacate space to the buffer and send vertex data to the cpu. it also sends data to our gpu
	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * numVertices), vertexData, GL_STATIC_DRAW);
	//GL_STATIC_DRAW gives the compiler a hint that this buffer is for drawing
	//if you wanted to change the contence of the buffer at run time you would use GL_DYNAMIC_DRAW.

	//define position atributes (tells us how to handel the float data from the vertices)
	//position artibute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	////color atribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return vao;
};

//creates the shader takes in the type and a referenece to the soure code 
unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	unsigned int shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
		return -1;
	}
	return shader;
};

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	int success;

	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();

	//creates and attaches shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;

};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	//glfw handdels the creation of a window and inputs
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	//checks to make sure that there are header files
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//finds all of the gl functions. all fuctions should start with gl -- commuitcating from the cpu to the gpu
	// 
	//Vertex Array Object (VAO)
	unsigned int vao = createVAO(vertices, 21);
	//create shader program
	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	//this is our render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();//input
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//clears all the colors
		glClear(GL_COLOR_BUFFER_BIT);

		// draw calls go here
		
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		float time = (float)glfwGetTime();
		int timeLocation = glGetUniformLocation(shaderProgram, "_Time");

		glUniform1f(timeLocation, time);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

