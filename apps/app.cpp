#include <iostream>


#include <glad.h>
#include <GLFW/glfw3.h>

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

int main()
{
    auto glWindow = OpenGlSetup();

    if(glWindow != nullptr){

        while(!glfwWindowShouldClose(glWindow)) {
            glClear(GL_COLOR_BUFFER_BIT);
            processInput(glWindow);
            glfwSwapBuffers(glWindow);
            glfwPollEvents();
        }
   }
   glfwTerminate();
   return 0;
}
