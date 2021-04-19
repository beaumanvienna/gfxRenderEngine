/* Engine Copyright (c) 2021 Engine Development Team 
   https://github.com/beaumanvienna/gfxRenderEngine

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

#include <unistd.h>

#include "engine.h"
#include "platform.h"
#include "shader.h"
#include "OpenGL/GL.h"

bool initGLFW();
bool initGLEW();

int main(int argc, char* argv[])
{
    std::cout << std::endl;
    std::cout << "Starting engine (gfxRenderEngine) v" << ENGINE_VERSION << std::endl;
    std::cout << std::endl;
    
    GLFWwindow* gWindow;
    
    // init glfw
    if (!initGLFW())
    {
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
    
    // set the number of screen updates to wait from the time glfwSwapBuffers 
    // was called before swapping the buffers
    glfwSwapInterval(1); // wait for next screen update
    
    // create verticies
    /*
     * ( -0.5f,  0.5f) (  0.0f,  0.5f) (  0.5f,  0.5f)
     * ( -0.5f,  0.0f) (  0.0f,  0.0f) (  0.5f,  0.0f)
     * ( -0.5f, -0.5f) (  0.0f, -0.5f) (  0.5f, -0.5f)
     * 
    */
    
    const uint NUMBER_OF_FLOATS_PER_VERTEX = 2;
    const uint NUMBER_OF_VERTICIES = 4;
    const float verticies[NUMBER_OF_FLOATS_PER_VERTEX][NUMBER_OF_VERTICIES] = 
    {
        -0.5f,  0.5f,
         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };
    
    // create indicies
    uint indicies[] =
    {
        0,1,3, /*first triangle */
        
        1,2,3  /* second triangle */
    };
    
    //create vertex array object (vao)
    uint vao;
    const uint NUMBER_OF_VAO_NAMES = 1;
    GLCall(glGenVertexArrays(NUMBER_OF_VAO_NAMES, &vao));
    GLCall(glBindVertexArray(vao));
    
    //create vertex buffer object (vbo)
    const uint NUMBER_OF_VBO_NAMES = 1;
    uint vbo[NUMBER_OF_VBO_NAMES];
    GLCall(glGenBuffers(NUMBER_OF_VBO_NAMES, vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo[0]));
    // load data into vbo
    GLCall(glBufferData
    (
        GL_ARRAY_BUFFER,                                                    /* target */
        sizeof(float) * NUMBER_OF_FLOATS_PER_VERTEX * NUMBER_OF_VERTICIES,  /* buffer size */
        (const void*)verticies,                                             /* actual data */
        GL_STATIC_DRAW                                                      /* usage */
    ));
    
    // specify vertex attributes (position at index 0, color color at index 1, normal at index 2, etc.)
    enum attrib_index
    {
        ATTRIB_INDEX_POSITION = 0,
        ATTRIB_INDEX_COLOR,
        ATTRIB_INDEX_NORMAL
    };
    GLCall(glVertexAttribPointer
    (
        ATTRIB_INDEX_POSITION,             /* index in vao */               /* index of the generic vertex attribute to be modified */
        NUMBER_OF_FLOATS_PER_VERTEX,                                        /* number of components per generic vertex attribute */
        GL_FLOAT,                                                           /* data type */
        false,                                                              /* normailzed */
        sizeof(float) * NUMBER_OF_FLOATS_PER_VERTEX,                        /* data size per drawing object (consecutive generic vertex attributes) */
        (const void*)0                                                      /* offset in vbo */
    ));
    //enable vertex attribute(s)
    GLCall(glEnableVertexAttribArray(ATTRIB_INDEX_POSITION));
    
    //create index buffer object (ibo)
    const uint NUMBER_OF_IBO_NAMES = 1;
    const uint NUMBER_OF_OBJECTS = 2; // number of triangles
    const uint NUMBER_OF_VERTICIES_PER_OBJECT = 3; // three verticies per triangle
    uint ibo[NUMBER_OF_IBO_NAMES];
    
    GLCall(glGenBuffers(NUMBER_OF_IBO_NAMES, ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]));
    // load data into ibo
    GLCall(glBufferData
    (
        GL_ELEMENT_ARRAY_BUFFER,                                            /* target */
        sizeof(int) * NUMBER_OF_OBJECTS * NUMBER_OF_VERTICIES_PER_OBJECT,   /* buffer size */
        (const void*)indicies,                                              /* actual data */
        GL_STATIC_DRAW                                                      /* usage */
    ));
    
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
    int shaderProgram = CreateShader(vertextShader, fragmentShader);
    
    if (gShaderStatus != SHADER_OK)
    {
        std::cout << "Shader creation failed" << std::endl;
        return -1;
    }
    GLCall(int colorUniformLocation = glGetUniformLocation(shaderProgram,"u_Color"));
    ASSERT(colorUniformLocation != -1);
    
    //detach everything
    GLCall(glBindVertexArray(INVALID_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, INVALID_ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INVALID_ID));
    glUseProgram(INVALID_ID);
    
    float red = 0.0f;
    const float INCREMENT = 0.01f;
    float delta = INCREMENT;
    while (!glfwWindowShouldClose(gWindow))
    {
        // compute animation
        if (red >= 1.0f) 
        {
            delta = -INCREMENT;
        }
        else if ((red <= 0.0f) )
        {
            delta = INCREMENT;
        }
        red += delta;
        
        //clear
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        // enable buffers and shaders
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo[0]));
        glUseProgram(shaderProgram);
        
        // set uniforms
        GLCall(glUniform4f(colorUniformLocation, red,0.1f,0.2f,1.0f));

        GLCall(glDrawElements
        (
            GL_TRIANGLES,                                           /* mode */
            NUMBER_OF_OBJECTS * NUMBER_OF_VERTICIES_PER_OBJECT,     /* count */
            GL_UNSIGNED_INT,                                        /* type */
            (void*)0                                                /* element array buffer offset */
        ));
        
        usleep(16667); // 60 frames per second (in micro (!) seconds)
        GLCall(glfwSwapBuffers(gWindow));

        glfwPollEvents();
    }
    
    GLCall(glDeleteProgram(shaderProgram));
    
    glfwTerminate();
    return 0;
};


bool initGLFW()
{
    
    // init glfw
    if (!glfwInit())
    {
        std::cout << "glfwInit() failed" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

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
