#include "Render.h"
#include <string>
#include <iostream>

std::string GlGetError(GLenum errorCode) // error code should be 1280 - 1285
{                                               // or 0x0500 - 9x0505
    std::string errorArray[] = {"GL_INVALID_ENUM", "GL_INVALID_VALUE",
    "GL_INVALID_OPERATION", "GL_STACK_OVERFLOW", "GL_STACK_UNDERFLOW", "GL_OUT_OF_MEMORY"};
    // subtracting by 1280 or 0x0500
    return errorArray[errorCode - 1280];
}

void GlClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GlLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        if (error > 1279 && error < 1286)
        std::cout << "[OpenGL Error] {" << GlGetError(error) << "}: " << function
                  << " " << file << ":" << line << std::endl;
        else
        std::cout << "[OpenGL Error] {" << "0x" << std::hex << error << "}: " << function
                  << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
