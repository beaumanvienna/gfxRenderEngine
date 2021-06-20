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

#include <cmath> 

#include "overlay.h"
#include "input.h"
#include "renderer.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "matrix.h"
#include <gtx/transform.hpp>
#include "controller.h"

bool drawWalkArea = false;

void Overlay::OnAttach() 
{ 
    // horn
    m_SpritesheetHorn.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_HORN), 25 /* frames */, 500 /* milliseconds per frame */, 2.0f /* scale) */);
    m_HornAnimation = m_SpritesheetHorn.GetSpriteAnimation();
    m_HornAnimation->Start();
    
    m_SpritesheetWalk.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALK), 6 /* frames */, 150 /* milliseconds per frame */, 2.0f /* scale) */);
    m_WalkAnimation = m_SpritesheetWalk.GetSpriteAnimation();
    m_WalkAnimation->Start();
    m_GuybrushWalkDelta = 33*4.3f;
    
    m_SpritesheetWalkUp.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALKUP), 6 /* frames */, 150 /* milliseconds per frame */, 2.0f /* scale) */);
    m_WalkUpAnimation = m_SpritesheetWalkUp.GetSpriteAnimation();
    m_WalkUpAnimation->Start();
    m_GuybrushWalkUpDelta = 12.0f;
    
    m_SpritesheetWalkDown.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALKDOWN), 6 /* frames */, 150 /* milliseconds per frame */, 2.0f /* scale) */);
    m_WalkDownAnimation = m_SpritesheetWalkDown.GetSpriteAnimation();
    m_WalkDownAnimation->Start();
    m_GuybrushWalkDownDelta = 12.0f;
    
    m_WhiteTexture = Texture::Create();
    int whitePixel = 0xffffffff;
    m_WhiteTexture->Init(1, 1, &whitePixel);
    m_WhiteSprite = new Sprite(0, 0.0f, 0.0f, 1.0f, 1.0f, m_WhiteTexture->GetWidth(), m_WhiteTexture->GetHeight(), m_WhiteTexture, "white texture", 100.0f, 100.0f);
    
    m_Translation = glm::vec3(462.0f, -264.0f, 0.0f);

    m_WalkArea = new Tetragon(
                                    { 211.5f, -439.5f},  // leftBottom
                                    { 901.5f, -436.5f},  // rightBottom
                                    { 891.5f, -126.0f},  // rightTop 
                                    {-153.0f, -102.0f}); // leftTop
}

void Overlay::OnDetach() 
{
    if (m_WhiteSprite) delete m_WhiteSprite;
    if (m_WalkArea) delete m_WalkArea;
}

void Overlay::OnUpdate()
{
    if (drawWalkArea)
    {
        m_WhiteTexture->Bind();
        glm::mat4 position = m_WalkArea->GetScaleMatrix();
        glm::vec4 color(0.8f, 0.1f, 0.1f, 0.5f);
        m_Renderer->Draw(m_WhiteSprite, position, -0.09f, false, color);
        
    }
    
    bool m_IsWalking = false;
    float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();

    // rotate based on controller input
    if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_LEFTSHOULDER)) 
    {
        m_Rotation += m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }
    else if (Input::IsControllerButtonPressed(Controller::FIRST_CONTROLLER, Controller::BUTTON_RIGHTSHOULDER)) 
    {
        m_Rotation -= m_RotationSpeed * Engine::m_Engine->GetTimestep();
    }

    // translation based on controller input
    glm::vec2 leftStick  = Input::GetControllerStick(Controller::FIRST_CONTROLLER, Controller::LEFT_STICK);


    //depth
    float depth, scaleDepth;
    depth = ((LIMIT_UP-m_Translation.y) / (-LIMIT_DOWN + LIMIT_UP)); // 0.0f to 1.0f
    scaleDepth = (1.0f + 0.65f * depth) * 0.6f;

    glm::vec3 translation = m_Translation;
    float frameTranslationX = m_FrameTranslationX;
    bool moveRight = false;
    bool isWalking = false;
    bool stickDeflectionX = (abs(leftStick.x) > 0.1) && (abs(leftStick.x) > abs(leftStick.y));
    bool stickDeflectionY = (abs(leftStick.y) > 0.1) && (abs(leftStick.y) > abs(leftStick.x));

    if (stickDeflectionX)
    {
        if (leftStick.x > 0)
        {
            moveRight = true;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                translation.x += m_GuybrushWalkDelta;
                frameTranslationX = 0.0f;
            }
            else
            {
                frameTranslationX = m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation->GetFrames()) * m_WalkAnimation->GetCurrentFrame();
            }
        }
        else
        {
            moveRight = false;
            if (!m_WalkAnimation->IsRunning()) 
            {
                m_WalkAnimation->Start();
                translation.x -= m_GuybrushWalkDelta;
                frameTranslationX = 0.0f;
            }
            else
            {
                frameTranslationX = -m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation->GetFrames()) * m_WalkAnimation->GetCurrentFrame();
            }
        }
        
        if (m_WalkArea->IsInBounds(translation))
        {
            isWalking = true;
            m_Translation = translation;
            m_FrameTranslationX = frameTranslationX;
            
            m_SpritesheetWalk.BeginScene();
        
            Sprite* sprite = m_WalkAnimation->GetSprite();
            
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(m_Translation) * Scale(depthScaling);
            
            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();
    
            m_Renderer->Draw(sprite, position, -0.1f, !moveRight);
        }
    }
    else
    {
        m_WalkAnimation->Start();
    }
    
    if (stickDeflectionY && (leftStick.y > 0))
    {
        
        static uint prevFrame = 0;
        uint frame = m_WalkUpAnimation->GetCurrentFrame();
        if (prevFrame != frame)
        {
            translation.y += m_GuybrushWalkUpDelta;
            prevFrame = frame;
        }
        if (!m_WalkUpAnimation->IsRunning()) 
        {
            m_WalkUpAnimation->Start();
        }
        if (m_WalkArea->IsInBounds(translation))
        {
            isWalking = true;
            m_Translation = translation;
            m_SpritesheetWalkUp.BeginScene();
    
            Sprite* sprite = m_WalkUpAnimation->GetSprite();
    
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(m_Translation) * Scale(depthScaling);
    
            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();
    
            m_Renderer->Draw(sprite, position, -0.1f);
        }
    }
    else
    {
        m_WalkUpAnimation->Start();
    }

    if (stickDeflectionY && (leftStick.y < 0))
    {
        static uint prevFrame = 0;
        uint frame = m_WalkDownAnimation->GetCurrentFrame();
        if (prevFrame != frame)
        {
            translation.y -= m_GuybrushWalkDownDelta;
            prevFrame = frame;
        }
        if (!m_WalkDownAnimation->IsRunning()) 
        {
            m_WalkDownAnimation->Start();
        }
        
        if (m_WalkArea->IsInBounds(translation))
        {
            isWalking = true;
            m_Translation = translation;

            m_SpritesheetWalkDown.BeginScene();
            
            Sprite* sprite = m_WalkDownAnimation->GetSprite();
            
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(m_Translation) * Scale(depthScaling);
            
            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();
    
            m_Renderer->Draw(sprite, position, -0.1f);
        }
    }
    else
    {
        m_WalkDownAnimation->Start();
    }

    if (!isWalking)
    {
        if (!m_HornAnimation->IsRunning()) m_HornAnimation->Start();
        m_SpritesheetHorn.BeginScene();
    
        Sprite* sprite = m_HornAnimation->GetSprite();
    
        // model matrix
        glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
        glm::mat4 modelMatrix = Translate(m_Translation) * Scale(depthScaling);
        
        // transformed position
        glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();

        m_Renderer->Draw(sprite, position, -0.1f);
    }
    else
    {
        m_HornAnimation->Start();
    }
}

void Overlay::OnEvent(Event& event) 
{
    
    EventDispatcher dispatcher(event);
        
    dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event) 
        {
            OnControllerButtonPressed(event);
            return true;
        }
    );
        
    dispatcher.Dispatch<ControllerButtonReleasedEvent>([this](ControllerButtonReleasedEvent event) 
        {
            OnControllerButtonReleased(event);
            return true;
        }
    );

}

void Overlay::OnControllerButtonPressed(ControllerButtonPressedEvent& event)
{
    event.MarkAsHandled();
}

void Overlay::OnControllerButtonReleased(ControllerButtonReleasedEvent& event)
{
    event.MarkAsHandled();
}
