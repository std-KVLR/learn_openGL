#include "QS_OpenGL_lib/qs_ogl_lib.h"



using namespace QSOpenGL;





void GLFW::processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_W)){
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    }
    else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        static bool lineMode = false;
        lineMode = !lineMode;
        glPolygonMode(GL_FRONT_AND_BACK, (lineMode) ? GL_LINE : GL_FILL);
    }
    else if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

}

GLFWwindow* GLFW::setup(const size_t versionMajor, const size_t versionMinor,
                 size_t windWidth, size_t windHeight, const char* windTitle)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glWindow = glfwCreateWindow(windWidth, windHeight, windTitle, nullptr, nullptr);
    if(glWindow == nullptr) {
        throw GLFW_Error("Error in glfw setup function: glfw window pointer is not assignment");
    }

    glfwMakeContextCurrent(glWindow);
    glfwSetFramebufferSizeCallback(glWindow, [](GLFWwindow* window, int w, int h)
                                            {
                                                glViewport(0, 0, w, h);
                                            });

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw GLFW_Error("Error in glfw setup function: GLAD load failure");
    }

    return glWindow;
}

float GLFW::gen_random_color(float min, float max)
{
    static std::default_random_engine generator;
    static std::uniform_real_distribution ds(min, max);
    return ds(generator);
}


//Shaders

void Shader::compile(const char* fileName, ShaderType glCompileStatus)
{
    std::ifstream shaderFile(fileName, std::ios::binary);
    if(shaderFile.is_open())
    {
        std::string shaderSource ( (std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
        const char* shaderSourceBuf = shaderSource.c_str();
        m_shader = glCreateShader(glCompileStatus);
        glShaderSource(m_shader, 1, &shaderSourceBuf, NULL);
        glCompileShader(m_shader);
    }
    shaderFile.close();

    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &(GlErrors.successful));
    if(!GlErrors.successful){
        glGetShaderInfoLog(m_shader, GlErrors.maxLogSize, NULL, GlErrors.info);
        throw ShaderCompilationError(GlErrors.info);
    }
}


