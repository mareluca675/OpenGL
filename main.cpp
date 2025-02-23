#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex shader in the GLSL language (position)
const char* vertexSourceCode = 
                            "#version 330 core\n" // Version of OpenGL, profile
                            "layout (location = 0) in vec3 aPos;\n" // Input and location
                            "void main()\n"
                            "{\n"
                            " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Output
                            "}\0";

// Fragment shader in the GLSL language (color)
const char* fragmentShaderSourceCodeYellow = 
                                    "#version 330 core\n" // Version of OpenGL, profile
                                    "out vec4 FragColor;\n" // Output
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(1.0f, 1.0f, 0.8f, 1.0f);\n"
                                    "}\0";

const char* fragmentShaderSourceCodeRed =
                                    "#version 330 core\n" // Version of OpenGL, profile
                                    "out vec4 FragColor;\n" // Output
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
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
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);

    // Attaching the source code to the shader and compiling it
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceCodeYellow, NULL);
    glCompileShader(fragmentShaderYellow);

    // Checking glCompileShaderDebbug for fragmentShader
    glCompileShaderDebbug(fragmentShaderYellow);

    // Attaching the source code to the shader and compiling it
    glShaderSource(fragmentShaderRed, 1, &fragmentShaderSourceCodeRed, NULL);
    glCompileShader(fragmentShaderRed);

    // Checking glCompileShaderDebbug for fragmentShader
    glCompileShaderDebbug(fragmentShaderRed);

    // ------------------------------------------------- PROGRAM RED

    // Creating a shader program to link all the shaders together
    unsigned int shaderProgramYellow = glCreateProgram();

    // Attaching the previous shaders to the newly created shader program
    // as well as linking them
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // Checking glProgramLinkDebbug for shaderProgram
    glProgramLinkDebbug(shaderProgramYellow);

	// ------------------------------------------------- PROGRAM YELLOW

    // Creating a shader program to link all the shaders together
    unsigned int shaderProgramRed = glCreateProgram();

    // Attaching the previous shaders to the newly created shader program
    // as well as linking them
    glAttachShader(shaderProgramRed, vertexShader);
    glAttachShader(shaderProgramRed, fragmentShaderRed);
    glLinkProgram(shaderProgramRed);

    // Checking glProgramLinkDebbug for shaderProgram
    glProgramLinkDebbug(shaderProgramRed);

    // ------------------------------------------------- 

    // Deleting the shaders, since we no longer need them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderYellow);
    glDeleteShader(fragmentShaderRed);

    // ------------------------------------------------- VAO & VBO & EBO

    float vertices1[] = { // 3D Coords (vertices data)
        0.0f, 0.0f, 0.0f, // middle
        0.5f, 0.0f, 0.0f, // bottom right
        -0.5f, 0.0f, 0.0f, // bottom left
        -0.25f, 0.5f, 0.0f, // top left
        0.25f, 0.5f, 0.0f // top right
    };

    unsigned int indices1[] = { // Indexes of the vertices
        0, 1, 4, // first triangle
    };

    float vertices2[] = { // 3D Coords (vertices data)
        0.0f, 0.0f, 0.0f, // middle
        0.5f, 0.0f, 0.0f, // bottom right
        -0.5f, 0.0f, 0.0f, // bottom left
        -0.25f, 0.5f, 0.0f, // top left
        0.25f, 0.5f, 0.0f // top right
    };

    unsigned int indices2[] = { // Indexes of the vertices
        0, 2, 3 // second triangle
    };

    // Creating a Vertex Buffer Object (VBO) to manage the memory
    unsigned int VBO1, VBO2;
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);

    // Creating a Vertex Array Object (VAO) to store all the state configurations for each vertex
    unsigned int VAO1, VAO2;
    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);

    // Creating a Element Buffer Object (EBO) to store the indexes
    unsigned int EBO1, EBO2;
    glGenBuffers(1, &EBO1);
    glGenBuffers(1, &EBO2);

    // ------------------------------------------------- FIRST TRIANGLE SETUP

    // Binding the buffer to the GL_ARRAY_BUFFER target and copying the data (vertices) into to buffer's memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    // Binding the Vertex Array Object
    glBindVertexArray(VAO1);

    // Binding the buffer to the GL_ELEMENT_ARRAY_BUFFER target and copying the data (indices) into to buffer's memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    // Telling OpenGL how it should interpret the vertex data / Setting our vertices attributes pointers
    // The first parameter specifies which vertex attribute we want to configure
    // We specified the position using layout(location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // -------------------------------------------------

    // ------------------------------------------------- SECOND TRIANGLE SETUP

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ------------------------------------------------- 

    // Unbinding VAO & VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // -------------------------------------------------

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode

    // ------------------------------------------------- RENDER LOOP

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Processing input
        processInput(window);

        // Setting the color
        glClearColor(0.2f, 0.4f, 1.2f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activating the program
        glUseProgram(shaderProgramRed);

        // Binding the Vertex Array Object 1
        glBindVertexArray(VAO1);

        // Drawing the shape
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Activating the program
        glUseProgram(shaderProgramYellow);

        // Binding the Vertex Array Object 2
        glBindVertexArray(VAO2);

        // Drawing the shape
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Double buffer
        glfwSwapBuffers(window);

        // Checks if any events are triggered
        glfwPollEvents();
    }

    // -------------------------------------------------

    // De-allocating all the memory
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shaderProgramRed);
    glDeleteProgram(shaderProgramYellow);

    glfwTerminate(); // Clean all of GLFW's resources

    // -------------------------------------------------

    return 0;
}
