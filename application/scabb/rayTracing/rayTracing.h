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

#pragma once

#include <memory>

#include "layer.h"
#include "engine.h"
#include "buffer.h"
#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "scabb/rayTracing/aux.h"
#include "scabb/rayTracing/hittableList.h"
#include "scabb/rayTracing/camera.h"

namespace ScabbApp
{
    glm::color RayColor(const Ray& ray, const Hittable& world);

    class RayTracing : public Layer
    {

    public:

        RayTracing(std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<VertexBuffer> vertexBuffer, 
                        std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetMarley,
                        const std::string& name = "RayTracing")
            : Layer(name), m_IndexBuffer(indexBuffer), m_VertexBuffer(vertexBuffer),
              m_Renderer(renderer), m_SpritesheetMarley(spritesheetMarley),
              m_Progress(0.0f) {}

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate() override;

    private:

        void ReportProgress(float progress);
        static glm::color RayColor(const Ray& ray, const Hittable& world, int bounce);
        static void ComputeBlock(int start, int end, uint index, uint* data, uint threadNumber);
        void CopyBlock(int start, int end, uint index);

    private:
        std::shared_ptr<IndexBuffer>  m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Renderer> m_Renderer;
        SpriteSheet* m_SpritesheetMarley;

        std::shared_ptr<Texture> m_CanvasTexture;
        Sprite* m_ProgressIndicator;
        Sprite* m_Canvas;

        static HittableList m_World;
        static Camera m_Camera;

        float m_Progress;
        std::vector<std::thread> m_WorkerThreads;

        uint m_ThreadData[IMAGE_WIDTH * IMAGE_HEIGHT];
        uint m_TextureData[IMAGE_WIDTH * IMAGE_HEIGHT];
        uint m_NumRowsPerBlock;
        uint m_NumThreads;
        static bool m_Terminate;

    private:
        enum ThreadState
        {
            RUNNING,
            DONE,
            INACTIVE
        };
        static std::vector<ThreadState> m_ThreadState;

    };
}
