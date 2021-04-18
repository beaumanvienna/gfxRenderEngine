/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/marley

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation files
   (the "Software"), to deal in the Software without restriction,
   including without limitation the rights to use, copy, modify, merge,
   publish, distribute, sublicense, and/or sell copies of the Software,
   and to permit persons to whom the Software is furnished to do so,
   subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <iostream>

#include "engine.h"
#include "platform.h"
#include "shader.h"

#include <GL/glew.h>
#include "GLFW/glfw3.h"

bool initGLEW()
{
    bool ok;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        ok =false;
        std::cout << "glewInit failed with error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
        ok = true;
        std::cout << "Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
        
        if (GLEW_ARB_vertex_program)
        {
          std::cout << "ARB_vertex_program extension is supported" << std::endl;
        }
        
        if (GLEW_VERSION_1_3)
        {
          std::cout << "OpenGL 1.3 is supported" << std::endl;
        }
        
        if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
        {
          std::cout << "OpenGL 1.4 point sprites are supported" << std::endl;
        }
        
        if (glewGetExtension("GL_ARB_fragment_program"))
        {
          std::cout << "ARB_fragment_program is supported" << std::endl;
        }
        
        std::cout << "Using OpenGL version " << glGetString(GL_VERSION) << std::endl;
    }
    
    std::cout << std::endl;
    return ok;
}

int main(int argc, char* argv[])
{
    std::cout << std::endl;
    std::cout << "Starting engine (gfxRenderEngine) v" << ENGINE_VERSION << std::endl;
    std::cout << std::endl;
    
    GLFWwindow* gWindow;
    
    // init glfw
    if (!glfwInit())
    {
        std::cout << "glfwInit() failed" << std::endl;
        return -1;
    }
    
    // create main window
    gWindow = glfwCreateWindow(640,480,"Engine v" ENGINE_VERSION,NULL,NULL);
    if (!gWindow)
    {
        glfwTerminate();
        std::cout << "Failed to create main window" << std::endl;
        return -1;
    }
    
    // create context
    glfwMakeContextCurrent(gWindow);
    
    // init glew
    if (!initGLEW())
    {
        return -1;
    }
    
    // create verticies
    /*
     * ( -0.5f,  0.5f) (  0.0f,  0.5f) (  0.5f,  0.5f)
     * ( -0.5f,  0.0f) (  0.0f,  0.0f) (  0.5f,  0.0f)
     * ( -0.5f, -0.5f) (  0.0f, -0.5f) (  0.5f, -0.5f)
     * 
    */
    const unsigned int NUMBER_OF_FLOATS_PER_VERTEX = 2;
    const unsigned int NUMBER_OF_VERTICIES = 3;
    const float verticies[NUMBER_OF_FLOATS_PER_VERTEX][NUMBER_OF_VERTICIES] = 
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };
    
    //create vertex buffer object (vbo)
    const unsigned int NUMBER_OF_BUFFER_OBJECT_NAMES = 1;
    unsigned int buffer[NUMBER_OF_BUFFER_OBJECT_NAMES];
    glGenBuffers(NUMBER_OF_BUFFER_OBJECT_NAMES, buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    // load data into vbo
    glBufferData
    (
        GL_ARRAY_BUFFER,                                                    /* target */
        sizeof(float) * NUMBER_OF_FLOATS_PER_VERTEX * NUMBER_OF_VERTICIES,  /* buffer size */
        (const void*)verticies,                                             /* actual data */
        GL_STATIC_DRAW                                                      /* usage */
    );
    
    // specify vertex attributes (position at index 0, color color at index 1, normal at index 2, etc.)
    enum attrib_index
    {
        ATTRIB_INDEX_POSITION = 0,
        ATTRIB_INDEX_COLOR,
        ATTRIB_INDEX_NORMAL
    };
    glVertexAttribPointer
    (
        ATTRIB_INDEX_POSITION,                                              /* index of the generic vertex attribute to be modified */
        NUMBER_OF_FLOATS_PER_VERTEX,                                        /* number of components per generic vertex attribute */
        GL_FLOAT,                                                           /* data type */
        false,                                                              /* normailzed */
        sizeof(float) * NUMBER_OF_FLOATS_PER_VERTEX,                        /* data size per drawing object (consecutive generic vertex attributes) */
        (const void*)0                                                      /* offset in vbo */
    );
    //enable vertex attribute(s)
    glEnableVertexAttribArray(ATTRIB_INDEX_POSITION);
    
    // program the GPU
    std::string vertextShader, fragmentShader;
    bool shadersLoaded = false;
    shadersLoaded =  LoadShaderFromFile(vertextShader, "engine/shader/vertexShader.vert");
    shadersLoaded &= LoadShaderFromFile(fragmentShader, "engine/shader/fragmentShader.frag");
    
    if (!shadersLoaded)
    {
        std::cout << "Couldn't load shaders" << std::endl;
        return -1;
    }
    CreateShader(vertextShader, fragmentShader);
    
    if (gShaderStatus != SHADER_OK)
    {
        std::cout << "Shader creation failed" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(gWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES,0,3);  

        glfwSwapBuffers(gWindow);

        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
};
