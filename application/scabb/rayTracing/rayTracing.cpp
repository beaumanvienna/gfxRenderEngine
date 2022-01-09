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

#include <memory>

#include "core.h"
#include "instrumentation.h"
#include "scabb/rayTracing/aux.h"
#include "scabb/rayTracing/camera.h"
#include "scabb/rayTracing/rayTracing.h"
#include "scabb/rayTracing/material.h"
#include "scabb/rayTracing/sphere.h"
#include "matrix.h"

namespace ScabbApp
{

    glm::color RayColor(const Ray& ray, const Hittable& world, int bounce)
    {
        HitRecord record;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (bounce > BOUNCE_LIMIT)
        {
            return glm::color(0.0f, 0.0f, 0.0f);
        }

        if (world.Hit(ray, 0.0001f, INFINITY, record))
        {
            Ray scattered;
            glm::color attenuation;
            if (record.m_Material->Scatter(ray, record, attenuation, scattered))
            {
                return attenuation * RayColor(scattered, world, bounce+1);
            }
            return glm::color(0.0f, 0.0f, 0.0f);
        }
        glm::vec3 unitDirection = glm::normalize(ray.GetDirection());
        auto t = 0.5f*(unitDirection.y + 1.0f);
        return (1.0f - t)*glm::color(1.0f, 1.0f, 1.0f) + t*glm::color(0.5f, 0.7f, 1.0f);
    }

    void RayTracing::OnAttach() 
    {
        PROFILE_SCOPE("RayTracing::OnAttach() ");

        // world
        
        auto materialCamera = std::make_shared<Lambertian>(glm::color(0.1f, 0.1f, 0.1f));
        auto materialGround = std::make_shared<Lambertian>(glm::color(0.8f, 0.8f, 0.0f));
        auto materialCenter = std::make_shared<Lambertian>(glm::color(0.7f, 0.3f, 0.3f));
        auto materialLeft   = std::make_shared<Metal>(glm::color(0.8f, 0.8f, 0.8f));
        auto materialRight  = std::make_shared<Metal>(glm::color(0.8f, 0.6f, 0.2f));
        
        m_World.Push(std::make_shared<Sphere>(glm::point3( 0.0f,    0.0f, 0.3f),   0.29f, materialCamera));

        m_World.Push(std::make_shared<Sphere>(glm::point3( 0.0f, -100.5f, -1.0f), 100.0f, materialGround));
        m_World.Push(std::make_shared<Sphere>(glm::point3( 0.0f,    0.4f, -1.0f),   0.4f, materialCenter));
        m_World.Push(std::make_shared<Sphere>(glm::point3(-0.6f,   -0.15f, -1.0f),   0.3f, materialLeft));
        m_World.Push(std::make_shared<Sphere>(glm::point3( 0.8f,   -0.0f, -1.0f),   0.3f, materialRight));

        // Camera
        Camera cam;

        uint data[IMAGE_WIDTH * IMAGE_HEIGHT];
        uint index = 0;

        for (int j = IMAGE_HEIGHT-1; j >= 0; --j)
        {
            for (int i = 0; i < IMAGE_WIDTH; ++i)
            {
                glm::color pixelColor(0, 0, 0);
                for (int s = 0; s < SAMPLES_PER_PIXEL; ++s)
                {
                    auto u = (i + RandomFloat()) / (IMAGE_WIDTH - 1);
                    auto v = (j + RandomFloat()) / (IMAGE_HEIGHT - 1);
                    Ray ray = cam.GetRay(u, v);
                    pixelColor += RayColor(ray, m_World, 1);
                }
                pixelColor.x = std::sqrt(pixelColor.x * INV_SAMPLES_PER_PIXEL);
                pixelColor.y = std::sqrt(pixelColor.y * INV_SAMPLES_PER_PIXEL);
                pixelColor.z = std::sqrt(pixelColor.z * INV_SAMPLES_PER_PIXEL);
                uint intRed   = static_cast<uint>(255.999 * Clamp(pixelColor.x, 0.0, 0.999) );
                uint intGreen = static_cast<uint>(255.999 * Clamp(pixelColor.y, 0.0, 0.999) );
                uint intBlue  = static_cast<uint>(255.999 * Clamp(pixelColor.z, 0.0, 0.999) );
                uint intAlpha = 255;

                data[index] = intRed << 0 | intGreen << 8 | intBlue << 16 | intAlpha << 24;
                index++;
            }
        }

        m_CanvasTexture = Texture::Create();
        m_CanvasTexture->Init(IMAGE_WIDTH, IMAGE_HEIGHT, data);

        m_Canvas = new Sprite(0.0f, 0.0f, 1.0f, 1.0f, IMAGE_WIDTH, IMAGE_HEIGHT, m_CanvasTexture, "canvas", 2.0f);

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
