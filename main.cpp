#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader in the GLSL language
const char* vertexSourceCode = "#version 330 core\n" // Version of OpenGL, profile
							   "layout (location = 0) in vec3 aPos;\n" // Input and location
							   "void main()\n"
							   "{\n"
							   " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Output
							   "}\0";

// Fragment shader in the GLSL language
const char* fragmentShaderSourceCode = "#version 330 core\n" // Version of OpenGL, profile
								   "out vec4 FragColor;\n" // Output
								   "void main()\n"
								   "{\n"
								   " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\0";


void glCompileShaderDebbug(unsigned int shader) {
	int succes;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);

	if (!succes) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
}

void glProgramLinkDebbug(unsigned int program) {
	int succes;
	char infoLog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &succes);

	if (!succes) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
}

// Function for changing window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	// Initializing GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	// Checking window
	if (window == NULL) {
		std::cout << "Failed to create GLFW window.";
		glfwTerminate();
		return -1;
	}

	// We tell GLFW to make the context of our window the main context on the current thread
	glfwMakeContextCurrent(window);

	// We tell OpenGL we want to call this function on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Checking GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialiaze GLAD";
		return -1;
	}

	float vertices[] = { // 3D Coords
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int VBO; // Vertex Buffer Object
	glGenBuffers(1, &VBO); // Generating the buffer
	glBindBuffer(VBO, GL_ARRAY_BUFFER); // Binding the buffer to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copying the data (vertices) into to buffer's memory

	// Creating a shader object of type GL_VERTEX_SHADER
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	// Attaching the source code to the shader and compiling it
	glShaderSource(vertexShader, 1, &vertexSourceCode, NULL);
	glCompileShader(vertexShader);

	// Checking glCompileShader for vertexShader
	glCompileShaderDebbug(vertexShader);

	// Creating a shader object of type GL_FRAGMENT_SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attaching the source code to the shader and compiling it
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShader);

	// Checking glCompileShader for fragmentShader
	glCompileShaderDebbug(fragmentShader);

	// Creating a shader program to link all the shaders together
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Attaching the previous shaders to the newly created shader program
	// as well as linking them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Activating the program
	glUseProgram(shaderProgram);

	// Deleting the shaders, since we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Telling OpenGL how it should interpret the vertex data
	// The first paramter specifies which vertex attribute we want to configure
	// We specified the position using layout(location = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// Creating a Vertex Array Object (VAO) to store all the state configurations for each vertex
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Binding the Vertex Array Object
	glBindVertexArray(VAO);

	// Copying our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Setting our vertices attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window); // Processing the input

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); // Double buffer
		glfwPollEvents(); // Checks if any events are triggered
	}

	glfwTerminate(); // Clean all of GLFW's resources
	return 0;
}
