#include "IndexBuffer.h"
#include "Render.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /* Enable GLEW */
    if (glewInit() != GLEW_OK)
        std::runtime_error("glew failed");

    float vertexPostions[] = {
        -0.5f, -0.5f, // 0
        0.5f, -0.5f, // 1
        0.5f, 0.5f, // 2
        -0.5f, 0.5f, // 3
    };

    unsigned int indexData[] = {
        0, 1, 2,
        2, 3, 0
    };

    /* Make Vertex Array */
    VertexArray va;

    /* Make Vertex Buffer and fill it */
    VertexBuffer vb(vertexPostions, 4 * 2 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    /* Make Index Buffer and fill it */
    IndexBuffer ib(indexData, 6);

    Shader shader("../Basic.shader");
    shader.Bind();

    shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    /* Unbinds everything */
    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    float r = 0.0f;
    float inc = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        renderer.Clear();

        shader.Bind();
        shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        /* Vertex Buffer Draw Call */
        renderer.Draw(va, ib, shader);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
            inc = -0.05f;
        else if (r < 0.0f)
            inc = 0.05f;

        r += inc;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
