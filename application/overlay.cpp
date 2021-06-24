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
#include "application.h"
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
    float scaleHero = 2.0f;
    // horn
    m_SpritesheetHorn.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_HORN), 25 /* frames */, 500 /* milliseconds per frame */, scaleHero /* scale) */);
    m_HornAnimation = m_SpritesheetHorn.GetSpriteAnimation();
    m_HornAnimation->Start();
    
    m_SpritesheetWalk.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALK), 6 /* frames */, 150 /* milliseconds per frame */, scaleHero /* scale) */);
    m_WalkAnimation = m_SpritesheetWalk.GetSpriteAnimation();
    m_WalkAnimation->Start();
    // 66.0f is the movement in the sprite sheet animation, 
    // however, the hero taks a big step before the new sequence starts, 
    // same as between frame #3 and #4.
    // 30.0f is measured from eye to eye in frame #3 and #4
    // see "resources/aseprite/walk.png"
    m_GuybrushWalkDelta = 66.0f * scaleHero + 30.0f; 
    
    m_SpritesheetWalkUp.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALKUP), 6 /* frames */, 150 /* milliseconds per frame */, scaleHero /* scale) */);
    m_WalkUpAnimation = m_SpritesheetWalkUp.GetSpriteAnimation();
    m_WalkUpAnimation->Start();
    m_GuybrushWalkUpDelta = 12.0f;
    
    m_SpritesheetWalkDown.AddSpritesheetAnimation(m_SpritesheetMarley->GetSprite(0, I_WALKDOWN), 6 /* frames */, 150 /* milliseconds per frame */, scaleHero /* scale) */);
    m_WalkDownAnimation = m_SpritesheetWalkDown.GetSpriteAnimation();
    m_WalkDownAnimation->Start();
    m_GuybrushWalkDownDelta = 12.0f;
    
    m_WhiteTexture = Texture::Create();
    int whitePixel = 0xffffffff;
    m_WhiteTexture->Init(1, 1, &whitePixel);
    m_WhiteSprite = new Sprite(0, 0.0f, 0.0f, 1.0f, 1.0f, m_WhiteTexture->GetWidth(), m_WhiteTexture->GetHeight(), m_WhiteTexture, "white texture", 4.0f, 4.0f);

    m_Translation = Application::m_GameState->GetHeroPosition();    
    m_FrameTranslationX = 0.0f;

}

void Overlay::OnDetach() 
{
    if (m_WhiteSprite) delete m_WhiteSprite;
}

void Overlay::OnUpdate()
{
    Tetragon* walkArea = Application::m_GameState->GetWalkArea();
    
    if (drawWalkArea)
    {
        m_WhiteTexture->Bind();
        glm::mat4 position = walkArea->GetScaleMatrix();
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
    depth = ((LIMIT_UP-m_Translation->y) / (-LIMIT_DOWN + LIMIT_UP)); // 0.0f to 1.0f
    scaleDepth = (1.0f + 0.65f * depth) * 0.6f;

    glm::vec3 translation = *m_Translation;
    bool isWalking = false;
    bool stickDeflectionX = (abs(leftStick.x) > 0.1) && (abs(leftStick.x) > abs(leftStick.y));
    bool stickDeflectionY = (abs(leftStick.y) > 0.1) && (abs(leftStick.y) > abs(leftStick.x));

    if (stickDeflectionX)
    {
        bool moveRight = false;
        float frameTranslationX = m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation->GetFrames()) * m_WalkAnimation->GetCurrentFrame() * scaleDepth;
        
        if (leftStick.x > 0)
        {
            glm::vec2 movement(frameTranslationX, 0.0f);
            if (walkArea->MoveInArea(&translation, movement))
            {
                m_Translation->y = translation.y;
                isWalking = true;
                moveRight = true;
                if (!m_WalkAnimation->IsRunning()) 
                {
                    m_WalkAnimation->Start();
                    m_Translation->x += m_GuybrushWalkDelta * scaleDepth;
                    frameTranslationX = 0.0f;
                }
            }
        }
        else
        {
            glm::vec2 movement(-frameTranslationX, 0.0f);
            if (walkArea->MoveInArea(&translation, movement))
            {
                m_Translation->y = translation.y;
                isWalking = true;
                moveRight = false;
                if (!m_WalkAnimation->IsRunning()) 
                {
                    m_WalkAnimation->Start();
                    m_Translation->x -= m_GuybrushWalkDelta * scaleDepth;
                    frameTranslationX = 0.0f;
                }
                else
                {
                    frameTranslationX = -frameTranslationX;
                }
            }
        }
        
        if (isWalking)
        {
            // latch frame translation only when an actual movement is possible
            m_FrameTranslationX = frameTranslationX;
            
            // render transformed sprite
            m_SpritesheetWalk.BeginScene();
        
            Sprite* sprite = m_WalkAnimation->GetSprite();
            
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);
            
            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();
    
            m_Renderer->Draw(sprite, position, -0.1f, !moveRight);
        }
        else if (m_FrameTranslationX) // hero out of bounds
        {
            // flush walking sprite sheet frame translation into hero model
            m_Translation->x += m_FrameTranslationX;
            m_FrameTranslationX = 0.0f;
        }
    }
    else
    {
        m_WalkAnimation->Start();
        
        if (m_FrameTranslationX)
        {
            // flush walking sprite sheet frame translation into hero model
            m_Translation->x += m_FrameTranslationX;
            m_FrameTranslationX = 0.0f;
        }
    }
    // walk up
    if (stickDeflectionY && (leftStick.y > 0) && !isWalking)
    {
        glm::vec2 movement(0.0f, m_GuybrushWalkUpDelta);
        if (walkArea->MoveInArea(&translation, movement))
        {
            m_Translation->x = translation.x;
            isWalking = true;
            
            // start if not running
            if (!m_WalkUpAnimation->IsRunning()) 
            {
                m_WalkUpAnimation->Start();
            }
            
            // update model position for a new sprite sheet frame
            if (m_WalkUpAnimation->IsNewFrame())
            {
                m_Translation->y = translation.y;
            }
            
            // render transformed sprite
            m_SpritesheetWalkUp.BeginScene();
    
            Sprite* sprite = m_WalkUpAnimation->GetSprite();
    
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);
    
            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();
    
            m_Renderer->Draw(sprite, position, -0.1f);
        }
    }
    else
    {
        m_WalkUpAnimation->Start();
    }

    // walk down
    if (stickDeflectionY && (leftStick.y < 0) && !isWalking)
    {
        glm::vec2 movement(0.0f, -m_GuybrushWalkUpDelta);
        if (walkArea->MoveInArea(&translation, movement))
        {
            m_Translation->x = translation.x;
            isWalking = true;
            
            // start if not running
            if (!m_WalkDownAnimation->IsRunning()) 
            {
                m_WalkDownAnimation->Start();
            }
            
            // update model position for a new sprite sheet frame
            if (m_WalkDownAnimation->IsNewFrame())
            {
                m_Translation->y = translation.y;
            }
            
            // render transformed sprite
            m_SpritesheetWalkDown.BeginScene();
            
            Sprite* sprite = m_WalkDownAnimation->GetSprite();
            
            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);
            
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
        glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);
        
        // transformed position
        glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();

        m_Renderer->Draw(sprite, position, -0.1f);
    }
    else
    {
        m_HornAnimation->Start();
    }

    if (drawWalkArea)
    {
        glm::mat4 position = Scale({10.0f,10.0f,0.0f}) * m_WhiteSprite->GetScaleMatrix();
        
        m_Renderer->Draw(m_WhiteSprite, Translate(*m_Translation) * position, -0.5f);
        
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
