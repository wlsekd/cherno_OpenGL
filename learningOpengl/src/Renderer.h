#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
                  x;\
                  ASSERT(GLLogCall(#x,__FILE__,__LINE__))

void GLClearError();

//��ӡʵ�ʷ����Ĵ�����������ں������÷����Ĵ���
bool GLLogCall(const char* functon, const char* file, int line);

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
