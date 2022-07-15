#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include <GL/glew.h>
#include <string>

#define ASSERT(x)                                                              \
  if (!(x)) __builtin_trap();
#define GLCall(x)\
  GlClearError();\
  x;\
  if (!GlLogCall(#x, __FILE__, __LINE__)) __builtin_trap(); // __builtin_trap = compiler intrisic
                      // needs to be here so gdb don't get confused

std::string GlGetError(GLenum errorCode); // error code should be 1280 - 1285
void GlClearError();
bool GlLogCall(const char *function, const char *file, int line);

class Renderer
{
public:
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader &shader) const;
    void Clear() const;
};
