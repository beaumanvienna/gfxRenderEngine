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

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "imgui_engine.h"

#include "application.h"
#include "engine.h"
#include "platform.h"
#include "log.h"

#include "shader.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "renderer.h"
#include "texture.h"
#include "spritesheet.h"

int app_main(int argc, char* argv[], Engine& engine)
{
    {
        std::string infoMessage = "entering main application program";
        Log::GetLogger()->info(infoMessage);
    }
    const uint NUMBER_OF_VERTICIES = 12;

    //create vertex array object (vao)
    VertexArray vertexArray;

    //create empty vertex buffer object (vbo)
    VertexBuffer vertexBuffer(sizeof(VertexBuffer::Vertex) * NUMBER_OF_VERTICIES);

    VertexBufferLayout vertexBufferLayout;
    
    // push position floats into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_Position)/sizeof(float));
    
    // push texture coordinates floats into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_TextureCoordinates)/sizeof(float));

    // push texture index float into attribute layout
    vertexBufferLayout.Push<float>(member_size(VertexBuffer::Vertex,m_Index)/sizeof(float));

    vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);
    
    //create empty index buffer object (ibo)
    IndexBuffer indexBuffer;

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

    SpriteSheet spritesheet_marley;
    spritesheet_marley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");

    SpriteSheet spritesheet_splash;
    spritesheet_splash.AddSpritesheetAnimation("resources/splashscreen/splash_spritesheet2.png", 20, 200);
    SpriteAnimation* splash = spritesheet_splash.GetSpriteAnimation();

    const uint TEXTURE_SLOT_0 = 0;
    int textureIDs[2] = {TEXTURE_SLOT_0, TEXTURE_SLOT_0 + 1};
    shaderProg.setUniform1iv("u_Textures", 2, textureIDs);
    
    // create Renderer
    Renderer renderer(engine.GetWindow());
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
    const float scaleMainWindowAspectRatio = engine.GetWindowAspectRatio();

    // scale to original size
    float scaleSize;
    
    // scale it to always have the same physical size on the screen
    // independently of the resolution
    const float scaleResolution = 1.0f / engine.GetWindowScale();

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
    
    Sprite* sprite;
    glm::vec4 position1;
    glm::vec4 position2;
    glm::vec4 position3;
    glm::vec4 position4;
    glm::mat4 projectionMatrix;
    
    splash->Start();

    while (!engine.WindowShouldClose())
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
        indexBuffer.BeginDrawCall();
        
        if (splash->IsRunning()) 
        {
            static bool splashMessage = true;
            if (splashMessage)
            {
                splashMessage = false;
                std::string infoMessage = "splash is running";
                Log::GetLogger()->info(infoMessage);
            }

            spritesheet_splash.BeginDrawCall();
            // --- first image ---
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

                sprite = splash->GetSprite();

                pos1X = sprite->m_Pos1X; 
                pos1Y = sprite->m_Pos1Y; 
                pos2X = sprite->m_Pos2X;
                pos2Y = sprite->m_Pos2Y;
                
                // aspect ratio of image
                scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

                // scale to main window size
                scaleSize = 1.0f;

                // scale to original size
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
                
                float textureID = static_cast<float>(spritesheet_splash.GetTextureSlot());
        
                float verticies[] = 
                { /*   positions   */ /* texture coordinate */
                     position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                     position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                     position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                     position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
                };
                vertexBuffer.LoadBuffer(verticies, sizeof(verticies));
            }
        }
        else
        {
            
            static bool mainMessage = true;
            if (mainMessage)
            {
                mainMessage = false;
                std::string infoMessage = "main screen is running";
                Log::GetLogger()->info(infoMessage);
            }
            spritesheet_marley.BeginDrawCall();
            
            // --- first image ---
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

                sprite = spritesheet_marley.GetSprite(0, 46);

                pos1X = sprite->m_Pos1X; 
                pos1Y = sprite->m_Pos1Y; 
                pos2X = sprite->m_Pos2X;
                pos2Y = sprite->m_Pos2Y;

                // aspect ratio of image
                scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

                // scale to original size
                scaleSize = engine.GetWindowWidth() / (1.0f * sprite->m_Width);

                // scale to original size
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
                
                float textureID = static_cast<float>(spritesheet_marley.GetTextureSlot());
        
                float verticies[] = 
                { /*   positions   */ /* texture coordinate */
                     position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                     position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                     position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                     position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
                };
                vertexBuffer.LoadBuffer(verticies, sizeof(verticies));
            }
            
            // --- second image ---
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
                sprite = spritesheet_marley.GetSprite(0, 47);

                pos1X = sprite->m_Pos1X; 
                pos1Y = sprite->m_Pos1Y; 
                pos2X = sprite->m_Pos2X;
                pos2Y = sprite->m_Pos2Y;

                // aspect ratio of image
                scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

                // scale to original size
                scaleSize = engine.GetWindowWidth() / (1.0f * sprite->m_Width);

                // scale to original size
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

                float textureID = static_cast<float>(spritesheet_marley.GetTextureSlot());

                float verticies[] = 
                { /*   positions   */ /* texture coordinate */
                     position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                     position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                     position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                     position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
                };
                vertexBuffer.LoadBuffer(verticies, sizeof(verticies));
            }

            // --- third image ---
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

                sprite = spritesheet_marley.GetSprite(0, 36);

                pos1X = sprite->m_Pos1X; 
                pos1Y = sprite->m_Pos1Y; 
                pos2X = sprite->m_Pos2X;
                pos2Y = sprite->m_Pos2Y;

                // aspect ratio of image
                scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

                // scale to original size
                scaleSize = engine.GetWindowWidth() / (1.0f * sprite->m_Width);

                // scale to original size
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

                float textureID = static_cast<float>(spritesheet_marley.GetTextureSlot());

                float verticies[] = 
                { /*   positions   */ /* texture coordinate */
                     position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
                     position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
                     position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
                     position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
                };
                vertexBuffer.LoadBuffer(verticies, sizeof(verticies));
            }
            
            
        }

        //clear
        renderer.Clear();
        
        // write index buffer
        indexBuffer.EndDrawCall();

        shaderProg.Bind();
        renderer.Draw(vertexArray,indexBuffer,shaderProg);
        
        // update imgui widgets
        if (!splash->IsRunning())
        {
            ImguiUpdate(engine.GetWindow(), engine.GetScaleImguiWidgets());
        }
        
        renderer.SwapBuffers();
        
        glfwPollEvents();
    }
    
    {
        std::string infoMessage = "leaving main application program";
        Log::GetLogger()->info(infoMessage);
    }

    return 0;
};
