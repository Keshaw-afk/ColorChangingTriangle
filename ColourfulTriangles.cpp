#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cmath>

void framebuffer_size_callback(GLFWwindow*, int, int);
void processInput(GLFWwindow* window);

const char* loadVertexShaderSource()
{
    std::ifstream vertexShaderFile("../Shaders/VertexShader.glsl");
    if (!vertexShaderFile.is_open())
    {
        std::cerr << "Vertex Shader source file could not be opened" << "\n";
    }
    std::stringstream buffer;

    buffer << vertexShaderFile.rdbuf();

    std::string file_contents = buffer.str();

    char* c_string = new char[file_contents.size()+1];
    std::strcpy(c_string, file_contents.c_str());

    return c_string;
}
const char* loadFragmentShaderSource()
{
    std::ifstream fragmentShaderFile("../Shaders/FragmentShader.glsl");
    if (!fragmentShaderFile.is_open())
    {
        std::cerr << "Vertex Shader source file could not be opened" << "\n";
    }
    std::stringstream buffer;

    buffer << fragmentShaderFile.rdbuf();

    std::string file_contents = buffer.str();

    char* c_string = new char[file_contents.size()+1];
    std::strcpy(c_string, file_contents.c_str());

    return c_string;
}

const unsigned int screen_width = 800;
const unsigned int screen_height = 800;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Hello Triangle",
                                          NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Window was not created" <<std::endl;
        glfwTerminate();
        return -1; 
    }

    glfwMakeContextCurrent(window); //This line should always come before glad initialization

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = loadVertexShaderSource();
    
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    delete[] vertexShaderSource;

    int success_vertex_shader;
    char infoLogVertexShader[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertex_shader);

    if (!success_vertex_shader)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVertexShader);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogVertexShader << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = loadFragmentShaderSource();

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    delete[] fragmentShaderSource;

    int success_fragment_shader;
    char infoLogFragmentShader[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragment_shader);

    if (!success_fragment_shader)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragmentShader);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragmentShader << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success_shader_program;
    char infoLogShaderProgram[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_shader_program);

    if (!success_shader_program)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogShaderProgram);
        std::cout << "ERROR:SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLogShaderProgram << std::endl;
    }

    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.8f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f; 
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.5f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

