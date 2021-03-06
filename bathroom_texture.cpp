#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL2.h>
using namespace std;
const char* vertex_shader =

"#version 430 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 textures;\n"
"out vec2 evt;\n"
"void main()\n"
"{\n"
"evt = textures;\n"
"gl_Position = vec4(position, 1.0);\n"
"}\0";
const char* fragment_shader =
"#version 430 core\n"
"out vec4 new_textures;\n"
"in vec2 evt;\n"
"uniform sampler2D sampTexture;\n" //variable for empty_Texture to put data in
"void main()\n"
"{\n"
"new_textures = texture(sampTexture, evt);\n" // if you set it to zero, it won't run
"}\n\0";
int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "RECTANGLE", NULL, NULL);
	if (window == NULL)
	{
		cout << "error";

	};
	
	glfwMakeContextCurrent(window);
	glewInit();
	glViewport(0, 0, 800, 600);



	int vertex = glCreateShader(GL_VERTEX_SHADER);
	int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char* pointer_to_shader[1];
	pointer_to_shader[0] = vertex_shader;
	glShaderSource(vertex, 1, pointer_to_shader, 0);
	
	
	pointer_to_shader[0] = fragment_shader;
	glShaderSource(fragment, 1, pointer_to_shader, 0);
	glCompileShader(vertex);
	glCompileShader(fragment);
	int program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);
	//store only the unique vertices and then specify the order at which
	//we want to draw these vertices in.
	
	float vertices[] = {
		// Positions        // Texture Coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 0
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 2
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // 3
	};

	int elements[] = {
		0, 1, 2, // first	
		2, 3, 0, // second	
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //we can't set zero to stride because our array is not tightly packed with one attribute
	// each coordinate in each vertex takes up 4 bytes. That means it takes 20 bytes each line
	//just setting it to zero will confused opengl into not knowing where to separate the data
	
	//STRIDE IS AMOUNT OF BYTES BETWEEN EACH VERTEX ATTRIB
	//void* is a memory address without a type.
	//stride of an array (also referred to as increment, pitch or step size) is the number of locations in memory between beginnings of successive array elements
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//3 *size of float is just where it starts
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	int width, height, nrchannels;
	unsigned char* data = SOIL_load_image("resources/bathroom.jpg", &width, &height, &nrchannels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D); //BINDS THE ACTUAL TEXTURE TO THE SQUARE
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.7f, 0.5f, 2.0f);
		glClear(GL_COLOR_BUFFER_BIT); 
		
		
		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
	
		glUseProgram(program);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
