#pragma once

#include <stdexcept>


namespace QSOpenGl
{

class ShaderCompilationError: public std::runtime_error {
public:
    ShaderCompilationError(const char* err)
        :std::runtime_error(err) {}
};

class Shader
{
public:
    enum ShaderType {GL_fragment_shader = GL_FRAGMENT_SHADER, GL_vertex_shader = GL_VERTEX_SHADER};

    Shader() = default;
    ~Shader() { glDeleteShader(m_shader); }

    void compile(const char* fileName, ShaderType glCompileStatus);

    unsigned get() const noexcept { return m_shader; }
    void deleteShader() const noexcept { this->~Shader(); }



protected:
    Shader(const char* fileName, const ShaderType type)
    {
        compile(fileName, type);
    }

    unsigned m_shader;

};


class VertexShader final: public Shader
{
public:
    VertexShader(const char* fileName)
        :Shader(fileName, ShaderType::GL_vertex_shader){}
private:
    void compile(const char* fileName, ShaderType glCompileStatus);
};

class FragmentShader final: public Shader
{
public:
    FragmentShader(const char* fileName)
        :Shader(fileName, ShaderType::GL_fragment_shader){}
private:
    void compile(const char* fileName, ShaderType glCompileStatus);
};


}
