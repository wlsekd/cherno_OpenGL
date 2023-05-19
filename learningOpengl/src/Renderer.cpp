#include "Renderer.h"


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

//打印实际发生的错误或者所有在函数调用发生的错误
bool GLLogCall(const char* functon, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " <<
            functon << " " << file << ":" << line <<
            std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
