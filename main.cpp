#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cassert>
#include "Render.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

/* OpenGL Notes
 * -------------
 * Error Checking
 * before func run, clear
 * run func
 * after check error
 *
 * Vertex = points of geo of triangles
 * Vertices can contain a lot more than just position
 * Vertex Array - binds attribs and buffers together
 * Vertex Buffer
 * Vertex Shader
 *
 * Indicies allow reuse of vertices
 * Index Buffer = stores indices of vertices
 * Index Shader
 *
 * Fragments = "pixels"
 * generally color
 * Fragment Buffer
 * Fragment Shader
 *
 * Uniforms set per draw
 * basically variables that can hook into shader code
 */

struct ShaderProjectSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProjectSource ParseShader(const std::string& filePath) {

    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    /* Gets and splits file into shader and vertex parts */
    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << " shader!" << std::endl;
        std::cout << message << std::endl;
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Set GLFW Context to Core */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Enable GLEW */
    if(glewInit() != GLEW_OK)
        std::runtime_error("glew failed");

    float vertexPostions[] = {
      -0.5f, -0.5f, // 0
       0.5f, -0.5f, // 1
       0.5f,  0.5f, // 2
      -0.5f,  0.5f, // 3
    };

    unsigned int indexData[] = {
      0, 1, 2,
      2, 3, 0
    };

    /* Make Vertex Array */
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    glBindVertexArray(vao);

    /* Make Vertex Buffer and fill it */
    VertexBuffer vb(vertexPostions, 4 * 2 * sizeof(float));

    /* Tell OpenGL about Vertex buffer contents */
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

    /* Make Index Buffer and fill it */
    IndexBuffer ib(indexData, 6);

    ShaderProjectSource source = ParseShader("../Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

    float r = 0.0f;
    float inc = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));


        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
        /* Vertex Buffer Draw Call */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            inc = -0.05f;
        else if (r < 0.0f)
            inc = 0.05f;

        r+=inc;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
