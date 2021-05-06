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

extern float debugTranslationX;
extern float debugTranslationY;
extern bool showGuybrush;
extern bool showTabIcons;

bool Application::Init(Engine* engine)
{
    EngineApp::Init(engine);

    spritesheet_marley.AddSpritesheetPPSSPP("resources/images/ui_atlas/ui_atlas.png");

    spritesheet_splash.AddSpritesheetAnimation("resources/splashscreen/splash_spritesheet2.png", 20, 200);
    splash = spritesheet_splash.GetSpriteAnimation();

    spritesheet_horn.AddSpritesheetAnimation("resources/sprites2/horn.png", 25, 500);
    hornAnimation = spritesheet_horn.GetSpriteAnimation();

    splash->Start();
    hornAnimation->Start();

    return true;
}

void Application::Run()
{

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
            orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
            orthoRight  = ortho_right  * scaleTextureX * scaleSize;
            orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
            orthoTop    = ortho_top    * scaleTextureY * scaleSize;
            
            glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
        
            //combine all matrixes
            glm::mat4 model_view_projection = projectionMatrix;
        
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
        
        // --- clouds ---
        {

            sprite = spritesheet_marley.GetSprite(0, 46);

            pos1X = sprite->m_Pos1X; 
            pos1Y = sprite->m_Pos1Y; 
            pos2X = sprite->m_Pos2X;
            pos2Y = sprite->m_Pos2Y;

            // aspect ratio of image
            scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

            // scale to original size
            scaleSize = m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);

            // scale to original size
            orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
            orthoRight  = ortho_right  * scaleTextureX * scaleSize;
            orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
            orthoTop    = ortho_top    * scaleTextureY * scaleSize;

            glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
            
            static float cloudX, whole;
            
            cloudX = glfwGetTime() / 300;
            cloudX = std::modf(cloudX, &whole) * 2.0f;
            
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
                
                // model matrix
                glm::mat4 modelMatrix(1.0f);
                
                //view matrix
                glm::mat4 viewMatrix(1.0f);
                
                glm::vec3 translation(cloudX, 0, 0);

                modelMatrix = glm::translate(glm::mat4(1.0f),translation);

                //combine all matrixes
                glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;

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
            {
                //fill index buffer object (ibo)
                indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);
                
                // model matrix
                glm::mat4 modelMatrix(1.0f);
                
                //view matrix
                glm::mat4 viewMatrix(1.0f);
                
                glm::vec3 translation(cloudX-2.0f, 0, 0);

                modelMatrix = glm::translate(glm::mat4(1.0f),translation);

                //combine all matrixes
                glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;

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
            scaleSize = m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);

            // scale to original size
            orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
            orthoRight  = ortho_right  * scaleTextureX * scaleSize;
            orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
            orthoTop    = ortho_top    * scaleTextureY * scaleSize;

            glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);

            //combine all matrixes
            glm::mat4 model_view_projection = projectionMatrix;

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
        if (showTabIcons)
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
            scaleSize = m_Engine->GetWindowWidth() / (1.0f * sprite->m_Width);
        
            // scale to original size
            orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
            orthoRight  = ortho_right  * scaleTextureX * scaleSize;
            orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
            orthoTop    = ortho_top    * scaleTextureY * scaleSize;
            
            
            // model matrix
            glm::mat4 modelMatrix(1.0f);
            
            //view matrix
            glm::mat4 viewMatrix(1.0f);
        
            glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);
        
            glm::vec3 translation(0, 0, 0);
            
            modelMatrix = glm::translate(glm::mat4(1.0f),translation);

            //combine all matrixes
            glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;
        
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
    
    // --- endless loop horn ---
    if (showGuybrush)
    {
        if (!hornAnimation->IsRunning()) hornAnimation->Start();
        spritesheet_horn.BeginDrawCall();
        //fill index buffer object (ibo)
        indexBuffer.AddObject(IndexBuffer::INDEX_BUFFER_QUAD);

        sprite = hornAnimation->GetSprite();

        pos1X = sprite->m_Pos1X; 
        pos1Y = sprite->m_Pos1Y; 
        pos2X = sprite->m_Pos2X;
        pos2Y = sprite->m_Pos2Y;
        
        // aspect ratio of image
        scaleTextureY = sprite->m_Width / (1.0f * sprite->m_Height);

        // scale to main window size
        scaleSize = m_Engine->GetWindowWidth() / (3.0f * sprite->m_Width);

        // scale to original size
        orthoLeft   = ortho_left   * scaleTextureX * scaleSize;
        orthoRight  = ortho_right  * scaleTextureX * scaleSize;
        orthoBottom = ortho_bottom * scaleTextureY * scaleSize;
        orthoTop    = ortho_top    * scaleTextureY * scaleSize;
        
        
        // model matrix
        glm::mat4 modelMatrix(1.0f);
        
        //view matrix
        glm::mat4 viewMatrix(1.0f);

        glm::mat4 projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, ortho_near, ortho_far);          
        glm::vec3 translation(0, 0, 0);
        translation.x =  0.5f  + debugTranslationX;
        translation.y = -0.75f + debugTranslationY;
        modelMatrix = glm::translate(glm::mat4(1.0f),translation);

        //combine all matrixes
        glm::mat4 model_view_projection = modelMatrix * viewMatrix * projectionMatrix;

        position1 = model_view_projection * normalizedPosition[0];
        position2 = model_view_projection * normalizedPosition[1];
        position3 = model_view_projection * normalizedPosition[2];
        position4 = model_view_projection * normalizedPosition[3];

        float textureID = static_cast<float>(spritesheet_horn.GetTextureSlot());

        float verticies[] = 
        { /*   positions   */ /* texture coordinate */
             position1[0], position1[1], pos1X, pos1Y, textureID, //    0.0f,  1.0f,
             position2[0], position2[1], pos2X, pos1Y, textureID, //    1.0f,  1.0f, // position 2
             position3[0], position3[1], pos2X, pos2Y, textureID, //    1.0f,  0.0f, 
             position4[0], position4[1], pos1X, pos2Y, textureID  //    0.0f,  0.0f  // position 1
        };
        vertexBuffer.LoadBuffer(verticies, sizeof(verticies));
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
        ImguiUpdate((GLFWwindow*)m_Engine->GetWindow(), m_Engine->GetScaleImguiWidgets());
    }
    
    renderer.SwapBuffers();
}
