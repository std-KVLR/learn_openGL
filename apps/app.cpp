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
        myOpenGL::FragmentShader fgshOrange;
        myOpenGL::FragmentShader fgshGold;
        try
        {
            vxsh.compile("../../shaders/vertex_shader1.glsl");
            fgshOrange.compile("../../shaders/fragment_shader1.glsl");
            fgshGold.compile("../../shaders/fragment_shader2.glsl");

        }
        catch(myOpenGL::Shader::ShaderCompilationError& err)
        {
            std::cerr << err.what << std::endl;
            return -1;
        }

        myOpenGL::GlError err;
        unsigned int shaderProgramOrange = glCreateProgram();
        glAttachShader(shaderProgramOrange, vxsh.get());
        glAttachShader(shaderProgramOrange, fgshOrange.get());
        glLinkProgram(shaderProgramOrange);
        glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &err.successful);
        if(!err.successful){
            glGetProgramInfoLog(shaderProgramOrange, err.maxLogSize, NULL, err.info);
            std::cerr << err.info << std::endl;
        }
        glDeleteShader(fgshOrange.get());

        unsigned int shaderProgramGold = glCreateProgram();
        glAttachShader(shaderProgramGold, vxsh.get());
        glAttachShader(shaderProgramGold, fgshGold.get());
        glLinkProgram(shaderProgramGold);
        glGetProgramiv(shaderProgramGold, GL_LINK_STATUS, &err.successful);
        if(!err.successful){
            glGetProgramInfoLog(shaderProgramGold, err.maxLogSize, NULL, err.info);
            std::cerr << err.info << std::endl;
        }
        glDeleteShader(fgshOrange.get());

        glDeleteShader(vxsh.get());


        //setup
        float vertices1[] =
        {
            -1.0f, -0.5f, 0.0f,
            -0.5f,  1.0f, 0.0f,
            -0.0f, -0.5f, 0.0f
        };

        float vertices2[] =
        {
            -0.0f, -0.5f, 0.0f,
             0.5f,  1.0f, 0.0f,
             1.0f, -0.5f, 0.0f
        };
        unsigned indicies[] =
        {
            0, 5, 3,
            1, 2, 4
        };

        unsigned VAO1, VAO2, VBO1, VBO2, EBO;

        glGenVertexArrays(1, &VAO1);
        glGenBuffers(1, &VBO1);
        glGenVertexArrays(2, &VAO2);
        glGenBuffers(2, &VBO2);
        //glGenBuffers(1, &EBO);

        auto config_buffers = [](unsigned& vao, unsigned& vbo, auto& vert)
                                {
                                    glBindVertexArray(vao);
                                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                                    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

                                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
                                    glEnableVertexAttribArray(0);

                                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                                    glBindVertexArray(0);


                                };

         config_buffers(VAO1, VBO1, vertices1);
         config_buffers(VAO2, VBO2, vertices2);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        //rendering

//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        while(!glfwWindowShouldClose(glWindow))
        {
            myOpenGL::processInput(glWindow);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgramOrange);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glUseProgram(shaderProgramGold);
            glBindVertexArray(VAO2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(glWindow);
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO1);
        glDeleteVertexArrays(1, &VAO2);
        glDeleteBuffers(1, &VBO1);
        glDeleteBuffers(1, &VBO2);
    }

   glfwTerminate();
   return 0;
}
