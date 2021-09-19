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

#include "marley/imguiOverlay.h"
#include "imgui_engine.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "orthographicCameraController.h"

extern float gCPUtimePerFrame;

namespace MarleyApp
{

    extern bool showTabIcons;
    bool showGuybrush = true;
    extern float duration;

    extern bool drawWalkArea;
    bool debugUI = false;
    bool showTileMap = false;

    void ImguiOverlay::OnAttach() 
    { 
    }

    void ImguiOverlay::OnDetach() 
    {

    }

    void ImguiOverlay::OnUpdate() 
    {

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Parameters");  
        ImGui::SetWindowFontScale(Engine::m_Engine->GetScaleImguiWidgets());
        const ImVec2& local_pos = {0,10};
        ImGui::SetCursorPos(local_pos);

        ImGui::Text("");
        ImGui::Text("Engine debug widget");
        ImGui::Checkbox("Show Guybrush", &showGuybrush);
        ImGui::Checkbox("Show tab icons", &showTabIcons);
        ImGui::Checkbox("Show walk area", &drawWalkArea);
        ImGui::Checkbox("debug user interface", &debugUI);
        ImGui::Checkbox("show tile map", &showTileMap);

        ImGui::SliderFloat("duration", &duration, 1.0f, 10.0f);
        ImGui::SliderFloat("zoom factor", &zoomFactor, 0.5f, 10.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("CPU time per frame: %.3f ms", gCPUtimePerFrame);
        ImGui::End();


        // Rendering
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void ImguiOverlay::OnEvent(Event& event)  {}
}
