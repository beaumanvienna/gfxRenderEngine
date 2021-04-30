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
#include "stb_image.h"
#include "spritesheet.h"

bool InitGLFW();
bool InitGLEW();
bool InitSDL();
bool CreateMainWindow(GLFWwindowPtr& mainWindow, int& windowWidth, int& windowHeight, float& windowScale, float& windowAspectRatio);

const int INVALID_ID = 0;


int main(int argc, char* argv[])
{
    GLFWwindowPtr gWindow;
    float gWindowScale;
    float gWindowAspectRatio;
    int gWindowWidth, gWindowHeight;
        
    // init logger
    if (!Log::Init())
    {
        std::cout << "Could initialize logger" << std::endl;
        return -1;
    }
    
    std::cout << std::endl;
    std::string infoMessage = "Starting engine (gfxRenderEngine) v" ENGINE_VERSION;
    Log::GetLogger()->info(infoMessage);
    std::cout << std::endl;
    
    // init glfw
    if (!InitGLFW())
    {
        return -1;
    }

    // create main window
    if (!CreateMainWindow(gWindow, gWindowWidth, gWindowHeight, gWindowScale, gWindowAspectRatio))
    {
        return -1;
    }
    
    // init glew
    if (!InitGLEW())
    {
        return -1;
    }
    
    // init SDL
    if (!InitSDL())
    {
        return -1;
    }
    
    // init imgui
    float gScaleImguiWidgets = gWindowScale * 1.4f; 
    if (!ImguiInit(gWindow, gScaleImguiWidgets))
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
    
    struct Vertex
    {
        float m_Position[2]; // 2D
        float m_TextureCoordinates[2]; 
    };
    
    const uint NUMBER_OF_VERTICIES = 12;

    // create indicies
    uint indicies[] =
    {
        0,1,3,   /*first triangle */
        
        1,2,3,   /* second triangle */
                
        4,5,7,   /*first triangle */
        
        5,6,7,   /* second triangle */
        
        8,9,11,  /*first triangle */
        
        9,10,11  /* second triangle */
    };

    { //buffers need to run out of scope before glfwTerminate
        
        //create vertex array object (vao)
        VertexArray vertexArray;

        //create empty vertex buffer object (vbo)
        VertexBuffer vertexBuffer(sizeof(Vertex) * NUMBER_OF_VERTICIES);

        VertexBufferLayout vertexBufferLayout;
        // push position floats into attribute layout
        
        vertexBufferLayout.Push<float>(member_size(Vertex,m_Position)/sizeof(float));
        // push texture coordinates floats into attribute layout
        vertexBufferLayout.Push<float>(member_size(Vertex,m_TextureCoordinates)/sizeof(float));
        vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

        //create index buffer object (ibo)
        IndexBuffer indexBuffer(indicies,sizeof(indicies)/sizeof(uint));

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
        
        SpriteSheet spritesheet;
        spritesheet.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");
        SpriteAnimation spritesheetAnimation
        (
            *spritesheet.GetSprite(0, 36),
            2
        );
        spritesheet.AddSpritesheetAnimation(spritesheetAnimation);
        //spritesheet.ListSprites();
        
        const uint TEXTURE_SLOT_0 = 0;
        Texture texture("resources/images/ui_atlas/ui_atlas.png");
        texture.Bind(TEXTURE_SLOT_0);
        shaderProg.setUniform1i("u_Texture", TEXTURE_SLOT_0);
        
        // create Renderer
        Renderer renderer;
        renderer.EnableBlending();

        // detach everything
        vertexBuffer.Unbind();
        vertexArray.Unbind();
        indexBuffer.Unbind();
        shaderProg.Unbind();

        // set up animation
        float red = 0.0f;
        const float INCREMENT = 0.01f;
        float delta = INCREMENT;
        
        // --- model, view, projection matrix ---
        
        // model matrix
        glm::mat4 modelMatrix(1.0f);
        
        //view matrix
        glm::mat4 viewMatrix(1.0f);
        
        // projection matrix
        // orthographic matrix for projecting two-dimensional coordinates onto the screen

        // normalize to -0.5f - 0.5f
        const float normalizeX = 0.5f;
        const float normalizeY = 0.5f;

        // aspect ratio of image
        const float scaleTextureX = 1.0f;
        float scaleTextureY;

        // aspect ratio of main window 
        const float scaleMainWindowAspectRatio = gWindowAspectRatio; 

        // scale to original size
        float scaleSize;
        
        // scale it to always have the same physical size on the screen
        // independently of the resolution
        const float scaleResolution = 1.0f / gWindowScale;

        float ORTHO_LEFT   =-normalizeX * scaleResolution;
        float ORTHO_RIGHT  = normalizeX * scaleResolution;
        float ORTHO_BOTTOM =-normalizeY * scaleResolution * scaleMainWindowAspectRatio;
        float ORTHO_TOP    = normalizeY * scaleResolution * scaleMainWindowAspectRatio;
        const float ORTHO_NEAR   =  1.0f;
        const float ORTHO_FAR    = -1.0f;

        float orthoLeft;
        float orthoRight;
        float orthoBottom;
        float orthoTop;
        
        float pos1X;
        float pos1Y; 
        float pos2X; 
        float pos2Y; 
        
        // MVB matrix
        glm::mat4 model_view_projection;

        glm::mat4 normalizedPosition
        (
            -0.5f,  0.5f, 0, 0,
             0.5f,  0.5f, 0, 0,
             0.5f, -0.5f, 0, 0,
            -0.5f, -0.5f, 0, 0
        );

        glm::vec3 translation(0, 0, 0);

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

            vertexBuffer.BeginDrawCall();

            Sprite* sprite = spritesheet.GetSprite(0, 46);

            pos1X = sprite->m_Pos1X; 
            pos1Y = sprite->m_Pos1Y; 
            pos2X = sprite->m_Pos2X;
            pos2Y = sprite->m_Pos2Y;

            // aspect ratio of image
            scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

            // scale to original size
            scaleSize = gWindowWidth / (1.0f * sprite->m_Width);

            // scale to original size
            scaleSize         = gWindowWidth / (1.0f * sprite->m_Width);
            orthoLeft   = ORTHO_LEFT   * scaleTextureX * scaleSize;
            orthoRight  = ORTHO_RIGHT  * scaleTextureX * scaleSize;
            orthoBottom = ORTHO_BOTTOM * scaleTextureY * scaleSize;
            orthoTop    = ORTHO_TOP    * scaleTextureY * scaleSize;

            glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ORTHO_NEAR, ORTHO_FAR);

            //combine all matrixes
            model_view_projection = projectionMatrix;

            glm::vec4 position1 = model_view_projection * normalizedPosition[0];
            glm::vec4 position2 = model_view_projection * normalizedPosition[1];
            glm::vec4 position3 = model_view_projection * normalizedPosition[2];
            glm::vec4 position4 = model_view_projection * normalizedPosition[3];

            float verticies[] = 
            { /*   positions   */ /* texture coordinate */
                 position1[0], position1[1], pos1X, pos1Y, //    0.0f,  1.0f,
                 position2[0], position2[1], pos2X, pos1Y, //    1.0f,  1.0f, // position 2
                 position3[0], position3[1], pos2X, pos2Y, //    1.0f,  0.0f, 
                 position4[0], position4[1], pos1X, pos2Y  //    0.0f,  0.0f  // position 1
            };
            vertexBuffer.LoadBuffer(verticies, sizeof(verticies));

            sprite = spritesheet.GetSprite(0, 47);

            pos1X = sprite->m_Pos1X; 
            pos1Y = sprite->m_Pos1Y; 
            pos2X = sprite->m_Pos2X;
            pos2Y = sprite->m_Pos2Y;

            // aspect ratio of image
            scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

            // scale to original size
            scaleSize = gWindowWidth / (1.0f * sprite->m_Width);

            // scale to original size
            scaleSize   = gWindowWidth / (1.0f * sprite->m_Width);
            orthoLeft   = ORTHO_LEFT   * scaleTextureX * scaleSize;
            orthoRight  = ORTHO_RIGHT  * scaleTextureX * scaleSize;
            orthoBottom = ORTHO_BOTTOM * scaleTextureY * scaleSize;
            orthoTop    = ORTHO_TOP    * scaleTextureY * scaleSize;

            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ORTHO_NEAR, ORTHO_FAR);

            //combine all matrixes
            model_view_projection = projectionMatrix;

            position1 = model_view_projection * normalizedPosition[0];
            position2 = model_view_projection * normalizedPosition[1];
            position3 = model_view_projection * normalizedPosition[2];
            position4 = model_view_projection * normalizedPosition[3];

            float verticies2[] = 
            { /*   positions   */ /* texture coordinate */
                 position1[0], position1[1], pos1X, pos1Y,
                 position2[0], position2[1], pos2X, pos1Y,
                 position3[0], position3[1], pos2X, pos2Y,
                 position4[0], position4[1], pos1X, pos2Y 
            };
            vertexBuffer.LoadBuffer(verticies2, sizeof(verticies2));

            sprite = spritesheet.GetSprite(0, 36);

            pos1X = sprite->m_Pos1X; 
            pos1Y = sprite->m_Pos1Y; 
            pos2X = sprite->m_Pos2X;
            pos2Y = sprite->m_Pos2Y;

            // aspect ratio of image
            scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

            // scale to original size
            scaleSize = gWindowWidth / (1.0f * sprite->m_Width);

            // scale to original size
            scaleSize   = gWindowWidth / (1.0f * sprite->m_Width);
            orthoLeft   = ORTHO_LEFT   * scaleTextureX * scaleSize;
            orthoRight  = ORTHO_RIGHT  * scaleTextureX * scaleSize;
            orthoBottom = ORTHO_BOTTOM * scaleTextureY * scaleSize;
            orthoTop    = ORTHO_TOP    * scaleTextureY * scaleSize;

            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ORTHO_NEAR, ORTHO_FAR);

            //combine all matrixes
            model_view_projection = projectionMatrix;

            position1 = model_view_projection * normalizedPosition[0];
            position2 = model_view_projection * normalizedPosition[1];
            position3 = model_view_projection * normalizedPosition[2];
            position4 = model_view_projection * normalizedPosition[3];

            float verticies3[] = 
            { /*   positions   */ /* texture coordinate */
                 position1[0], position1[1], pos1X, pos1Y,
                 position2[0], position2[1], pos2X, pos1Y,
                 position3[0], position3[1], pos2X, pos2Y,
                 position4[0], position4[1], pos1X, pos2Y 
            };
            vertexBuffer.LoadBuffer(verticies3, sizeof(verticies3));

            //clear
            renderer.Clear();

            shaderProg.Bind();
            renderer.Draw(vertexArray,indexBuffer,shaderProg);
            /*
            // update imgui widgets
            ImguiUpdate(gWindow, gScaleImguiWidgets);
            */
            usleep(32000); // ~30 frames per second (in micro (!) seconds)
            GLCall(glfwSwapBuffers(gWindow));

            glfwPollEvents();
        }
    } //buffers need to run out of scope before glfwTerminate
    glfwTerminate();

    return 0;
};

bool CreateMainWindow(GLFWwindowPtr& mainWindow, int& windowWidth, int& windowHeight, float& windowScale, float& windowAspectRatio)
{
    bool ok = false;
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);
    
    windowWidth = videoMode->width / 1.5f;
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
            // set app icon
            GLFWimage icon;
            icon.pixels = stbi_load("resources/images/engine.png", &icon.width, &icon.height, 0, 4); //rgba channels 
            if (icon.pixels) 
            {
                glfwSetWindowIcon(mainWindow, 1, &icon); 
                stbi_image_free(icon.pixels);
            }
            else
            {
                std::cout << "Could not load app icon " << std::endl;
            }
            
            // set scaling and aspect ratio 
            windowScale = windowWidth / 1280.0f;
            windowAspectRatio = windowHeight / (1.0f * windowWidth);
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
        std::string infoMessage = "Using GLEW ";
        infoMessage += (char*)(glewGetString(GLEW_VERSION));
        Log::GetLogger()->info(infoMessage);
        
        if (GLEW_ARB_vertex_program)
        {
            Log::GetLogger()->info("ARB_vertex_program extension is supported");
        }
        
        if (GLEW_VERSION_1_3)
        {
            Log::GetLogger()->info("OpenGL 1.3 is supported");
        }
        
        if (glewIsSupported("GL_VERSION_1_4  GL_ARB_point_sprite"))
        {
            Log::GetLogger()->info("OpenGL 1.4 point sprites are supported");
        }
        
        if (glewGetExtension("GL_ARB_fragment_program"))
        {
            Log::GetLogger()->info("ARB_fragment_program is supported");
        }
        
        infoMessage = "Using OpenGL version ";
        infoMessage += (char*)glGetString(GL_VERSION);
        Log::GetLogger()->info(infoMessage);
    }
    
    std::cout << std::endl;
    return ok;
}

bool InitSDL()
{
    bool ok =false;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_JOYSTICK ) < 0 )
    {
        std::cout << "Could not initialize SDL. SDL Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        ok = true;
    }
        
    return ok;
}
