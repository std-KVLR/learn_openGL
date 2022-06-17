#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include <utility>
#include <random>
#include <chrono>
#include <thread>

#include "QS_OpenGL_lib/qs_ogl_lib.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad.h>


int main()
{
    try
    {
        GLFWwindow* glWindow = QSOpenGL::GLFW::setup(3, 3, 800, 600);
        //shaders
        QSOpenGL::VertexShader vxsh("../../shaders/vertex_shader1.glsl");
        QSOpenGL::VertexShader vxshwc("../../shaders/vertex_shaderwc1.glsl");
        QSOpenGL::FragmentShader fgshOrange("../../shaders/fragment_shader1.glsl");
        QSOpenGL::FragmentShader fgshGold("../../shaders/fragment_shader2.glsl");
        QSOpenGL::FragmentShader fgshLSGray("../../shaders/fragment_shader3.glsl");

        unsigned int shaderProgramOrange = glCreateProgram();
        glAttachShader(shaderProgramOrange, vxsh.get());
        glAttachShader(shaderProgramOrange, fgshOrange.get());
        glLinkProgram(shaderProgramOrange);
        glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &QSOpenGL::GlErrors.successful);
        if(!QSOpenGL::GlErrors.successful){
            glGetProgramInfoLog(shaderProgramOrange, QSOpenGL::GlErrors.maxLogSize, NULL, QSOpenGL::GlErrors.info);
            std::cerr << QSOpenGL::GlErrors.info << std::endl;
        }
        glDeleteShader(fgshOrange.get());

        unsigned int shaderProgramGold = glCreateProgram();
        glAttachShader(shaderProgramGold, vxsh.get());
        glAttachShader(shaderProgramGold, fgshGold.get());
        glLinkProgram(shaderProgramGold);
        glGetProgramiv(shaderProgramGold, GL_LINK_STATUS, &QSOpenGL::GlErrors.successful);
        if(!QSOpenGL::GlErrors.successful){
            glGetProgramInfoLog(shaderProgramGold, QSOpenGL::GlErrors.maxLogSize, NULL, QSOpenGL::GlErrors.info);
            std::cerr << QSOpenGL::GlErrors.info << std::endl;
        }
        glDeleteShader(fgshGold.get());

        unsigned int shaderProgramCustomColor = glCreateProgram();
        glAttachShader(shaderProgramCustomColor, vxshwc.get());
        glAttachShader(shaderProgramCustomColor, fgshLSGray.get());
        glLinkProgram(shaderProgramCustomColor);
        glGetProgramiv(shaderProgramCustomColor, GL_LINK_STATUS, &QSOpenGL::GlErrors.successful);
        if(!QSOpenGL::GlErrors.successful){
            glGetProgramInfoLog(shaderProgramCustomColor, QSOpenGL::GlErrors.maxLogSize, NULL, QSOpenGL::GlErrors.info);
            std::cerr << QSOpenGL::GlErrors.info << std::endl;
        }
        int vertexColorLocation = glGetUniformLocation(shaderProgramCustomColor, "ourColor");
        if(vertexColorLocation == -1) std::cerr << "Vertex color variable not found" << std::endl;
        glDeleteShader(fgshLSGray.get());

        glDeleteShader(vxsh.get());
        glDeleteShader(vxshwc.get());


        //setup
        float vertices1[] =
        {
            -1.0f, -0.5f, 0.0f,
            -0.5f,  1.0f, 0.0f,
            -0.0f, -0.5f, 0.0f
        };

        float vertices2[] =
        {
            //location              //colors
            -0.0f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
             0.5f,  1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
             1.0f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f
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
         //config_buffers(VAO2, VBO2, vertices2);
         glBindVertexArray(VAO2);
         glBindBuffer(GL_ARRAY_BUFFER, VBO2);
         glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(sizeof(float)*3));
         glEnableVertexAttribArray(1);

         glBindBuffer(GL_ARRAY_BUFFER, 0);
         glBindVertexArray(0);
         QSOpenGL::VertexShader vxsh2A("../../shaders/vertex_shader2.glsl");
         QSOpenGL::FragmentShader fgsh4("../../shaders/fragment_shader4.glsl");
         unsigned int shaderProgram2A = glCreateProgram();
         glAttachShader(shaderProgram2A, vxsh2A.get());
         glAttachShader(shaderProgram2A, fgsh4.get());
         glLinkProgram(shaderProgram2A);
         glGetProgramiv(shaderProgram2A, GL_LINK_STATUS, &QSOpenGL::GlErrors.successful);
         if(!QSOpenGL::GlErrors.successful){
             glGetProgramInfoLog(shaderProgram2A, QSOpenGL::GlErrors.maxLogSize, NULL, QSOpenGL::GlErrors.info);
             std::cerr << QSOpenGL::GlErrors.info << std::endl;
         }
         glDeleteShader(fgsh4.get());
         glDeleteShader(vxsh2A.get());

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        //rendering

//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);



        while(!glfwWindowShouldClose(glWindow))
        {
            QSOpenGL::GLFW::processInput(glWindow);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgramGold);
            glBindVertexArray(VAO1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glUseProgram(shaderProgram2A);
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
    catch(std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        return -1;
    }
    catch(...)
    {
        std:: cerr << "UNKNOWN ERROR..." << std::endl;
        return -1;
    }

   glfwTerminate();
   return 0;
}
