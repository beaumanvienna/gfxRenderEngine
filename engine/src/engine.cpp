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
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "renderer.h"
#include "texture.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "SDL.h"
#include "imgui_engine.h"

bool InitGLFW();
bool InitGLEW();
bool CreateMainWindow(GLFWwindowPtr& mainWindow, float& windowScale);

const int INVALID_ID = 0;

int main(int argc, char* argv[])
{
    GLFWwindowPtr gWindow;
    float gWindowScale;
    
    std::cout << std::endl;
    std::cout << "Starting engine (gfxRenderEngine) v" << ENGINE_VERSION << std::endl;
    std::cout << std::endl;
    
    // init glfw
    if (!InitGLFW())
    {
        return -1;
    }

    // create main window
    if (!CreateMainWindow(gWindow,gWindowScale))
    {
        return -1;
    }
    
    // init glew
    if (!InitGLEW())
    {
        return -1;
    }
    
    // init imgui
    if (!ImguiInit(gWindow, gWindowScale))
    {
        return -1;
    }

    // set the number of screen updates to wait from the time glfwSwapBuffers 
    // was called before swapping the buffers
    glfwSwapInterval(1); // wait for next screen update
    
    // create verticies
    /* positions
     * ( -0.5f,  0.5f) (  0.0f,  0.5f) (  0.5f,  0.5f)
     * ( -0.5f,  0.0f) (  0.0f,  0.0f) (  0.5f,  0.0f)
     * ( -0.5f, -0.5f) (  0.0f, -0.5f) (  0.5f, -0.5f)
     * 
    */
    
    /* texture coordinates
     * (  0.0f,  1.0f) (  0.5f,  1.0f) (  1.0f,  1.0f)
     * (  0.0f,  0.5f) (  0.5f,  0.5f) (  1.0f,  5.0f)
     * (  0.0f,  0.0f) (  0.5f,  0.0f) (  1.0f,  0.0f)
     * 
    */
    const uint NUMBER_OF_POSITION_FLOATS_PER_VERTEX = 2;
    const uint NUMBER_OF_TEX_COORD_FLOATS_PER_VERTEX = 2;
    const uint NUMBER_OF_FLOATS_PER_VERTEX = NUMBER_OF_POSITION_FLOATS_PER_VERTEX + NUMBER_OF_TEX_COORD_FLOATS_PER_VERTEX;
    const uint NUMBER_OF_VERTICIES = 4;
    const float verticies[NUMBER_OF_FLOATS_PER_VERTEX][NUMBER_OF_VERTICIES] = 
    { /*   positions   */ /* texture coordinate */
         -0.5f,  0.5f,          0.0f,  1.0f,
          0.5f,  0.5f,          1.0f,  1.0f,
          0.5f, -0.5f,          1.0f,  0.0f,
         -0.5f, -0.5f,          0.0f,  0.0f
    };

    // create indicies
    uint indicies[] =
    {
        0,1,3, /*first triangle */
        
        1,2,3  /* second triangle */
    };

    { //buffers need to run out of scope before glfwTerminate
        
        //create vertex array object (vao)
        VertexArray vertexArray;

        //create vertex buffer object (vbo)
        VertexBuffer vertexBuffer(verticies, sizeof(float) * NUMBER_OF_FLOATS_PER_VERTEX * NUMBER_OF_VERTICIES);

        VertexBufferLayout vertexBufferLayout;
        // push position floats into attribute layout
        vertexBufferLayout.Push<float>(NUMBER_OF_POSITION_FLOATS_PER_VERTEX);
        // push texture coordinates floats into attribute layout
        vertexBufferLayout.Push<float>(NUMBER_OF_TEX_COORD_FLOATS_PER_VERTEX);
        vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

        //create index buffer object (ibo)
        const uint NUMBER_OF_OBJECTS = 2; // number of triangles
        const uint NUMBER_OF_VERTICIES_PER_OBJECT = 3; // three verticies per triangle
        IndexBuffer indexBuffer(indicies,NUMBER_OF_OBJECTS * NUMBER_OF_VERTICIES_PER_OBJECT);

        // program the GPU
        ShaderProgram shaderProg;
        shaderProg.AddShader(GL_VERTEX_SHADER,   "engine/shader/vertexShader.vert");
        shaderProg.AddShader(GL_FRAGMENT_SHADER, "engine/shader/fragmentShader.frag");
        shaderProg.Create();
        
        if (!shaderProg.IsOK())
        {
            std::cout << "Shader creation failed" << std::endl;
            return -1;
        }
        
        // --- model, view, projection matrix ---
        
        // model matrix
        glm::mat4 modelMatrix(1.0f);
        
        //view matrix
        glm::mat4 viewMatrix(1.0f);
        
        // projection matrix
        // orthographic matrix for projecting two-dimensional coordinates onto the screen
        // set this according to the main window's aspect ratio
        const float ORTHO_LEFT   = -8.0f;
        const float ORTHO_RIGHT  =  8.0f;
        const float ORTHO_BOTTOM = -4.5f;
        const float ORTHO_TOP    =  4.5f;
        const float ORTHO_NEAR   =  1.0f;
        const float ORTHO_FAR    = -1.0f;
        glm::mat4 projectionMatrix = glm::ortho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_BOTTOM, ORTHO_TOP, ORTHO_NEAR, ORTHO_FAR);
        
        glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;
        shaderProg.setUniformMat4f("m_MVP", model_view_projection);
        
        Texture texture("resources/pictures/barrel.png");
        texture.Bind();
        const uint TEXTURE_SLOT_0 = 0;
        shaderProg.setUniform1i("u_Texture", TEXTURE_SLOT_0);

        //create Renderer
        Renderer renderer;
        renderer.EnableBlending();

        //detach everything
        vertexBuffer.Unbind();
        vertexArray.Unbind();
        indexBuffer.Unbind();
        shaderProg.Unbind();

        // set up animation
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
            renderer.Clear();
            
            // set uniforms
            shaderProg.Bind();
            renderer.Draw(vertexArray,indexBuffer,shaderProg);
            
            // update imgui widgets
            ImguiUpdate(gWindow, gWindowScale);
            
            usleep(16667); // 60 frames per second (in micro (!) seconds)
            GLCall(glfwSwapBuffers(gWindow));

            glfwPollEvents();
        }
    } //buffers need to run out of scope before glfwTerminate
    glfwTerminate();

    return 0;
};

bool CreateMainWindow(GLFWwindowPtr& mainWindow, float& windowScale)
{
    bool ok = false;
    int count;
    int windowWidth, windowHeight;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    
    windowWidth = videoMode->width / 1.5;
    windowHeight = windowWidth / 16 * 9;
    int monitorX, monitorY;
    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // make window invisible before it gets centered
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Engine v" ENGINE_VERSION, NULL, NULL);
    if (!mainWindow)
    {
        glfwTerminate();
        std::cout << "Failed to create main window" << std::endl;
    }
    else
    {
        // center window
        glfwSetWindowPos(mainWindow,
                         monitorX + (videoMode->width - windowWidth) / 2,
                         monitorY + (videoMode->height - windowHeight) / 2);
        
        // make the centered window visible
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwShowWindow(mainWindow);
        
        // create context
        char description[1024];
        glfwMakeContextCurrent(mainWindow);
        if (glfwGetError((const char**)(&description)) != GLFW_NO_ERROR)
        {
            std::cout << "could not create window context" << description << std::endl;
        }
        else
        {
            windowScale = windowWidth / 1280 *1.4f;
            // all good
            ok = true;
        }
    }
    return ok;
}


bool InitGLFW()
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

bool InitGLEW()
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
