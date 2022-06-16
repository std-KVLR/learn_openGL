#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <utility>
#include <glad.h>
#include <GLFW/glfw3.h>

namespace myOpenGL
{

struct GlError
{
    static constexpr int maxLogSize = 512;
    int successful;
    char info[maxLogSize];
};

class Shader
{
public:
    class ShaderCompilationError {
    public:
        const std::string what;
        ShaderCompilationError(const char* err)
            :what(err) {}

    };

protected:

    void compile_process(const char* fileName, unsigned glCompileStatus)
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
    }

    void compile_info()
    {
        GlError err;

        glGetShaderiv(m_shader, GL_COMPILE_STATUS, &(err.successful));

        if(!err.successful){
            glGetShaderInfoLog(m_shader, 512, NULL, err.info);
            throw ShaderCompilationError(err.info);
        }

    }
    unsigned m_shader;

public:
    virtual void compile(const char* fileName) = 0;
    virtual unsigned get() { return m_shader; }
};


class VertexShader final: public Shader
{
public:
    VertexShader():Shader(){}

    void compile(const char* fileName)
    {
        compile_process(fileName, GL_VERTEX_SHADER);
        compile_info();
    }
};

class FragmentShader final: public Shader
{
public:
    FragmentShader():Shader(){}

    void compile(const char* fileName)
    {
        compile_process(fileName, GL_FRAGMENT_SHADER);
    }
};

void processInput(GLFWwindow* window)
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

GLFWwindow* OpenGlSetup()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);

    if(window == nullptr) {
        std::cerr << "Fail" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h)
                                            {
                                                glViewport(0, 0, w, h);
                                            });


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Fail GLAD init" << std::endl;
        return nullptr;
    }

    return window;
};

}


int main()
{
    auto glWindow = myOpenGL::OpenGlSetup();

    if(glWindow != nullptr){

        //shaders
        myOpenGL::VertexShader vxsh;
        myOpenGL::FragmentShader fgsh;
        try
        {
            vxsh.compile("../../shaders/vertex_shader1.glsl");
            fgsh.compile("../../shaders/fragment_shader1.glsl");

        }
        catch(myOpenGL::Shader::ShaderCompilationError& err)
        {
            std::cerr << err.what << std::endl;
            return -1;
        }

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vxsh.get());
        glAttachShader(shaderProgram, fgsh.get());
        glLinkProgram(shaderProgram);
        myOpenGL::GlError err;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &err.successful);
        if(!err.successful){
            glGetProgramInfoLog(shaderProgram, err.maxLogSize, NULL, err.info);
            std::cerr << err.info << std::endl;
        }
        glUseProgram(shaderProgram);
        glDeleteShader(vxsh.get());
        glDeleteShader(fgsh.get());

        //setup
        float vertices[] =
        {
           -0.5f, -0.5f, 0.0f,
            0.5F, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };
        unsigned indicies[] =
        {
            0, 1, 3,
            1, 2, 3
        };

        unsigned VBO, VAO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        //delinking buffer and array
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);


        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        while(!glfwWindowShouldClose(glWindow))
        {
            myOpenGL::processInput(glWindow);
            glClear(GL_COLOR_BUFFER_BIT);
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(glWindow);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

   glfwTerminate();
   return 0;
}
