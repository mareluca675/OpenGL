#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader in the GLSL language (position)
const char* vertexSourceCode = 
                            "#version 330 core\n" // Version of OpenGL, profile
                            "layout (location = 0) in vec3 aPos;\n" // Input and location
	                        "layout (location = 1) in vec3 aColor;\n" // Input and locati
	                        "out vec3 ourColor;\n" // Output
                            "void main()\n"
                            "{\n"
	                        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" 
	                        " ourColor = aColor;\n"
                            "}\0";

// Fragment shader in the GLSL language (color)
const char* fragmentShaderSourceCode = 
                                    "#version 330 core\n" // Version of OpenGL, profile
                                    "out vec4 FragColor;\n" // Output
	                                "in vec3 ourColor;\n" // Input
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(ourColor, 1.0);\n"
                                    "}\0";


void glCompileShaderDebbug(unsigned int shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void glProgramLinkDebbug(unsigned int program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
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
    // ------------------------------------------------- GLFW & WINDOW

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating window
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
        std::cout << "Failed to initialize GLAD";
        return -1;
    }

    // ------------------------------------------------- SHADERS

    // Creating a shader object of type GL_VERTEX_SHADER
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attaching the source code to the shader and compiling it
    glShaderSource(vertexShader, 1, &vertexSourceCode, NULL);
    glCompileShader(vertexShader);

    // Checking glCompileShaderDebbug for vertexShader
    glCompileShaderDebbug(vertexShader);

    // Creating shader objects of type GL_FRAGMENT_SHADER
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attaching the source code to the shader and compiling it
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShader);

    // Checking glCompileShaderDebbug for fragmentShader
    glCompileShaderDebbug(fragmentShader);

    // ------------------------------------------------- PROGRAM RED

    // Creating a shader program to link all the shaders together
    unsigned int shaderProgram = glCreateProgram();

    // Attaching the previous shaders to the newly created shader program
    // as well as linking them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Checking glProgramLinkDebbug for shaderProgram
    glProgramLinkDebbug(shaderProgram);

	// ------------------------------------------------- 

    // Deleting the shaders, since we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ------------------------------------------------- VAO & VBO & EBO

    float vertices[] = {
        // positions       // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
    };


    unsigned int indices[] = { // Indexes of the vertices
        0, 1, 2, // first triangle
    };

    // Creating a Vertex Buffer Object (VBO) to manage the memory
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Creating a Vertex Array Object (VAO) to store all the state configurations for each vertex
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Creating a Element Buffer Object (EBO) to store the indexes
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // ------------------------------------------------- TRIANGLE SETUP

    // Binding the buffer to the GL_ARRAY_BUFFER target and copying the data (vertices) into to buffer's memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Binding the Vertex Array Object
    glBindVertexArray(VAO);

    // Binding the buffer to the GL_ELEMENT_ARRAY_BUFFER target and copying the data (indices) into to buffer's memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Telling OpenGL how it should interpret the vertex data / Setting our vertices attributes pointers
    // The first parameter specifies which vertex attribute we want to configure
    // We specified the position using layout(location = 0)

	// Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	// Colour attribute
    // Changed the offest to 3 * sizeof(float) 
	// since the colour data starts after the position data, which is a vec3 of floats
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // -------------------------------------------------

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode

    // ------------------------------------------------- RENDER LOOP

    // Activating the program before render loop
    // since we have only one
    glUseProgram(shaderProgram);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Processing input
        processInput(window);

        // Setting the color
        glClearColor(0.2f, 0.4f, 1.2f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Binding the Vertex Array Object 1
        glBindVertexArray(VAO);

        // Drawing the shape
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Double buffer
        glfwSwapBuffers(window);

        // Checks if any events are triggered
        glfwPollEvents();
    }

    // -------------------------------------------------

    // De-allocating all the memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate(); // Clean all of GLFW's resources

    // -------------------------------------------------

    return 0;
}
