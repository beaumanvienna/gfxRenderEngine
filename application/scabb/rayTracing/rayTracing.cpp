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

#include "scabb/rayTracing/rayTracing.h"
#include "scabb/rayTracing/aux.h"
#include "matrix.h"
#include "core.h"

namespace ScabbApp
{
    void RayTracing::OnAttach() 
    {
        static constexpr int IMAGE_WIDTH = 256;
        static constexpr int IMAGE_HEIGHT = 256;

        uint data[IMAGE_WIDTH * IMAGE_HEIGHT];
        uint index = 0;

        for (int j = IMAGE_HEIGHT-1; j >= 0; --j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                auto red   = double(i) / (IMAGE_WIDTH-1);
                auto green = double(j) / (IMAGE_HEIGHT-1);
                auto blue  = 0.25;

                uint intRed   = static_cast<uint>(255.999 * red);
                uint intGreen = static_cast<uint>(255.999 * green);
                uint intBlue  = static_cast<uint>(255.999 * blue);
                uint intAlpha = 255;

                data[index] = intRed << 0 | intGreen << 8 | intBlue << 16 | intAlpha << 24;
                index++;
            }
        }

        m_CanvasTexture = Texture::Create();
        m_CanvasTexture->Init(IMAGE_WIDTH, IMAGE_HEIGHT, data);

        m_Canvas = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, 256, 256, m_CanvasTexture, "canvas", 3.0f);

        m_ProgressIndicator = m_SpritesheetMarley->GetSprite(I_WHITE);

    }

    void RayTracing::OnDetach()
    {
        if (m_Canvas)
        {
            delete m_Canvas;
            m_Canvas = nullptr;
        }
    }

    void RayTracing::OnUpdate() 
    {
        m_CanvasTexture->Bind();
        glm::vec3 translation{0.0f, 0.0f, 0.0f};
        glm::mat4 position = Translate(translation) * m_Canvas->GetScaleMatrix();
        m_Renderer->Draw(m_Canvas, position);

        static constexpr float MAX_COUNT = 240;
        static float count = 0.0f;
        m_Progress = (count + 60) / MAX_COUNT;
        if (count < MAX_COUNT) count += 1.0f;

        ReportProgress(m_Progress);
    }

    void RayTracing::OnEvent(Event& event)
    {
    }

    void RayTracing::ReportProgress(float progress)
    {
        // clamp
        if (progress > 1.0f)
        {
            progress = 1.0f;
        }
        else if (progress < 0.0f)
        {
            progress = 0.0f;
        }

        // draw
        static float screenWidth = Engine::m_Engine->GetContextWidth();
        static float halfScreenWidth = Engine::m_Engine->GetContextWidth() / 2.0f;
        static float halfScreenHeight = Engine::m_Engine->GetContextHeight() / 2.0f;
        static float height = 20.0f;
        m_SpritesheetMarley->BeginScene();
        m_ProgressIndicator->SetScale(progress * screenWidth, height);
        glm::vec3 translation{-halfScreenWidth * (1.0f - progress), halfScreenHeight, 0.0f};
        glm::mat4 position = Translate(translation) * m_ProgressIndicator->GetScaleMatrix();
        m_Renderer->Draw(m_ProgressIndicator, position);
    }
}
