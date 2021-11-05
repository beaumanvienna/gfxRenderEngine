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

#include "renderer.h"
#include "marley/marley.h"
#include "marley/characters/GuybrushWalk.h"
#include "gtc/matrix_transform.hpp"
#include <gtx/transform.hpp>
#include "matrix.h"
#include "glm.hpp"

namespace MarleyApp
{

    bool drawWalkArea = false;

    void Overlay::OnAttach()
    {
        float scaleHero = 2.0f;
        // horn
        m_SpritesheetHorn.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_HORN), 25 /* frames */, scaleHero /* scale) */);
        m_HornAnimation.Create(500 /* milliseconds per frame */, &m_SpritesheetHorn);
        m_HornAnimation.Start();

        m_SpritesheetWalk.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_WALK), 6 /* frames */, scaleHero /* scale) */);
        m_WalkAnimation.Create(150 /* milliseconds per frame */, &m_SpritesheetWalk);
        m_WalkAnimation.Start();
        // 66.0f is the movement in the sprite sheet animation,
        // however, the hero taks a big step before the new sequence starts,
        // same as between frame #3 and #4.
        // 30.0f is measured from eye to eye in frame #3 and #4
        // see "resources/aseprite/walk.png"
        m_GuybrushWalkDelta = 66.0f * scaleHero + 30.0f;

        m_SpritesheetWalkUp.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_WALKUP), 6 /* frames */, scaleHero /* scale) */);
        m_WalkUpAnimation.Create(150 /* milliseconds per frame */, &m_SpritesheetWalkUp);
        m_WalkUpAnimation.Start();
        m_GuybrushWalkUpDelta = 12.0f;

        m_SpritesheetWalkDown.AddSpritesheetRow(m_SpritesheetMarley->GetSprite(I_WALKDOWN), 6 /* frames */, scaleHero /* scale) */);
        m_WalkDownAnimation.Create(150 /* milliseconds per frame */, &m_SpritesheetWalkDown);
        m_WalkDownAnimation.Start();
        m_GuybrushWalkDownDelta = 12.0f;

        m_WhiteSprite = m_SpritesheetMarley->GetSprite(I_WHITE);

        m_Translation = Marley::m_GameState->GetHeroPosition();
        m_FrameTranslationX = 0.0f;

        m_InputHandler = std::make_unique<InputHandler>(5.0f);

    }

    void Overlay::OnDetach()  {}

    void Overlay::OnUpdate()
    {
        Fade();
        if (m_Alpha == 0.0f) return;
        glm::vec4 transparency(1.0f, 1.0f, 1.0f, m_Alpha);
        Tetragon* walkArea = Marley::m_GameState->GetWalkArea();

        if (drawWalkArea)
        {
            glm::mat4 position = walkArea->GetScaleMatrix();
            glm::vec4 color(0.8f, 0.1f, 0.1f, 0.5f);
            m_Renderer->Draw(m_WhiteSprite, position, -0.09f, color);

        }

        bool m_IsWalking = false;
        float translationStep = m_TranslationSpeed * Engine::m_Engine->GetTimestep();

        // get input
        glm::vec2 movementCommand;
        m_InputHandler->GetMovement(movementCommand);
        m_InputHandler->GetRotation(m_Rotation);

        //depth
        float depth, scaleDepth;
        depth = ((LIMIT_UP-m_Translation->y) / (-LIMIT_DOWN + LIMIT_UP)); // 0.0f to 1.0f
        scaleDepth = (1.0f + 0.65f * depth) * 0.6f;

        glm::vec3 translation = *m_Translation;
        bool isWalking = false;

        if (movementCommand.x)
        {
            bool moveRight = false;
            float frameTranslationX = m_GuybrushWalkDelta / static_cast<float>(m_WalkAnimation.GetFrames()) * m_WalkAnimation.GetCurrentFrame() * scaleDepth;

            if (movementCommand.x > 0)
            {
                glm::vec2 movement(frameTranslationX, 0.0f);
                if (walkArea->MoveInArea(&translation, movement))
                {
                    m_Translation->y = translation.y;
                    isWalking = true;
                    moveRight = true;
                    if (!m_WalkAnimation.IsRunning())
                    {
                        m_WalkAnimation.Start();
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
                    if (!m_WalkAnimation.IsRunning())
                    {
                        m_WalkAnimation.Start();
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

                Sprite* sprite = m_WalkAnimation.GetSprite();

                // model matrix
                glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
                glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);

                // transformed position
                bool flip;
                if (sprite->m_Rotated)
                {
                    flip = moveRight;
                }
                else
                {
                    flip = !moveRight;
                }
                glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix(flip);

                m_Renderer->Draw(sprite, position, -0.1f, transparency);
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
            m_WalkAnimation.Start();

            if (m_FrameTranslationX)
            {
                // flush walking sprite sheet frame translation into hero model
                m_Translation->x += m_FrameTranslationX;
                m_FrameTranslationX = 0.0f;
            }
        }
        // walk up
        if (movementCommand.y && (movementCommand.y > 0) && !isWalking)
        {
            glm::vec2 movement(0.0f, m_GuybrushWalkUpDelta);
            if (walkArea->MoveInArea(&translation, movement))
            {
                m_Translation->x = translation.x;
                isWalking = true;

                // start if not running
                if (!m_WalkUpAnimation.IsRunning())
                {
                    m_WalkUpAnimation.Start();
                }

                // update model position for a new sprite sheet frame
                if (m_WalkUpAnimation.IsNewFrame())
                {
                    m_Translation->y = translation.y;
                }

                // render transformed sprite
                m_SpritesheetWalkUp.BeginScene();

                Sprite* sprite = m_WalkUpAnimation.GetSprite();

                // model matrix
                glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
                glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);

                // transformed position
                glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();

                m_Renderer->Draw(sprite, position, -0.1f, transparency);
            }
        }
        else
        {
            m_WalkUpAnimation.Start();
        }

        // walk down
        if (movementCommand.y && (movementCommand.y < 0) && !isWalking)
        {
            glm::vec2 movement(0.0f, -m_GuybrushWalkUpDelta);
            if (walkArea->MoveInArea(&translation, movement))
            {
                m_Translation->x = translation.x;
                isWalking = true;

                // start if not running
                if (!m_WalkDownAnimation.IsRunning())
                {
                    m_WalkDownAnimation.Start();
                }

                // update model position for a new sprite sheet frame
                if (m_WalkDownAnimation.IsNewFrame())
                {
                    m_Translation->y = translation.y;
                }

                // render transformed sprite
                m_SpritesheetWalkDown.BeginScene();

                Sprite* sprite = m_WalkDownAnimation.GetSprite();

                // model matrix
                glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
                glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);

                // transformed position
                glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();

                m_Renderer->Draw(sprite, position, -0.1f, transparency);
            }
        }
        else
        {
            m_WalkDownAnimation.Start();
        }

        if (!isWalking)
        {
            if (!m_HornAnimation.IsRunning()) m_HornAnimation.Start();
            m_SpritesheetHorn.BeginScene();

            Sprite* sprite = m_HornAnimation.GetSprite();

            // model matrix
            glm::vec3 depthScaling = glm::vec3(scaleDepth,scaleDepth,0);
            glm::mat4 modelMatrix = Translate(*m_Translation) * Scale(depthScaling);

            // transformed position
            glm::mat4 position = modelMatrix * Rotate( m_Rotation, glm::vec3(0, 0, 1) ) * sprite->GetScaleMatrix();

            m_Renderer->Draw(sprite, position, -0.1f, transparency);
        }
        else
        {
            m_HornAnimation.Start();
        }

        if (drawWalkArea)
        {
            glm::mat4 position = Scale({40.0f, 40.0f, 0.0f}) * m_WhiteSprite->GetScaleMatrix();

            m_Renderer->Draw(m_WhiteSprite, Translate(*m_Translation) * position, -0.5f, transparency);

        }
    }

    void Overlay::Fade()
    {
        if (m_FadeIn)
        {
            float timestep = Engine::m_Engine->GetTimestep();
            m_Alpha += timestep;
            if ( m_Alpha > 1.0f)
            {
                m_Alpha = 1.0f;
                m_FadeIn = false;
            }
        }
        else if (m_FadeOut)
        {
            float timestep = Engine::m_Engine->GetTimestep();
            m_Alpha -= timestep;
            if ( m_Alpha < 0.0f)
            {
                m_Alpha = 0.0f;
                m_FadeOut = false;
            }
        }
    }

    void Overlay::OnEvent(Event& event)
    {

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<ControllerButtonPressedEvent>([this](ControllerButtonPressedEvent event)
            {
                OnControllerButtonPressed(event);
                return false;
            }
        );

        dispatcher.Dispatch<ControllerButtonReleasedEvent>([this](ControllerButtonReleasedEvent event)
            {
                OnControllerButtonReleased(event);
                return false;
            }
        );
    }

    void Overlay::OnControllerButtonPressed(ControllerButtonPressedEvent& event) {}

    void Overlay::OnControllerButtonReleased(ControllerButtonReleasedEvent& event) {}
}
