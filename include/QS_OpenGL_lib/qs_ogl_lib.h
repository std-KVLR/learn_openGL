#ifndef QUICKSILVER_OPENGL_LIB
#define QUICKSILVER_OPENGL_LIB
#include <string>
#include <stdexcept>
#include <random>
#include <fstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>


namespace QSOpenGL
{

class ShaderCompilationError: public std::runtime_error {
public:
    ShaderCompilationError(const char* err)
        :std::runtime_error(err) {}
};

struct
{
    const int maxLogSize = 512;
    int successful;
    char info[512];

}GlErrors;



class Shader
{
public:
    enum ShaderType {GL_fragment_shader = GL_FRAGMENT_SHADER, GL_vertex_shader = GL_VERTEX_SHADER};

    Shader() = default;
    ~Shader(){}

    void compile(const char* fileName, ShaderType glCompileStatus);

    unsigned get() const { return m_shader;}

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


namespace GLFW
{

class GLFW_Error: public std::runtime_error
{
public:
    GLFW_Error(const char* errVal)
        :std::runtime_error(errVal) {}
};

void processInput(GLFWwindow* window);
GLFWwindow* setup(const size_t versionMajor, const size_t versionMinor, size_t windWidth,
                            size_t windHeight, const char* windTitle = "QS OpenGL Layer");
float gen_random_color(float min, float max);

};


}


#endif
