#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

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

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");

	//defining vertex postions

	float vertices[9] =
	{
		// all Z's zero becuase it is a 2d object
		//x //y //z
		-0.5, -0.5, 0.0, //bottem left
		0.5, -0.5, 0.0, // bottem right
		0.0, 0.5, 0.0 // top left

	};


	//Vertex Buffer Object (VBO) (used to send data to the GPU a buffer)
	//define new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	//after creating our new vbo buffer we then put it in the GL_ARRAY_BUFFER slot making it our new buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allacate space to the buffer and send vertex data to the cpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//GL_STATIC_DRAW gives the compiler a hint that this buffer is for drawing
	//if you wanted to change the contence of the buffer at run time you would use GL_DYNAMIC_DRAW.



	//move to a fucntion
	//Vertex Array Object (VAO)
	unsigned int vao;
	//why does this need more arguemnts?
	glGenVertexArrays(vao);

	glBindVertexArray(vao);
	//tell vao to pull data from vbo (what is vbo in the contex of this function?)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//define position atributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	//next step is tor refactor in to a function with a handle(what does this mean?) to a newley created VAO

	

}

unsigned int createVAO(float* vertexData, int numVertices)
{
	

};