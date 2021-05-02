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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "OpenGL/GL.h"

#define ENGINE_SCALE 2.5

bool showGuybrush = true;
bool showTabIcons = true;
float debugTranslationX = 0.0f;
float debugTranslationY = 0.0f;

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool ImguiInit(GLFWwindow* window, float windowScale)
{
        
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    
    // scale
    ImGui::GetStyle().ScaleAllSizes(windowScale);
    
    return true;
}

void ImguiUpdate(GLFWwindow* window, float windowScale)
{
    
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();    

    
    static int counter = 0;

    ImGui::Begin("Parameters");  
    ImGui::SetWindowFontScale(windowScale);
    const ImVec2& local_pos = {0,10};
    ImGui::SetCursorPos(local_pos);

    ImGui::Text("Engine debug widget");
    ImGui::Checkbox("Show Guybrush", &showGuybrush);
    ImGui::Checkbox("Show tab icons", &showTabIcons);

    ImGui::SliderFloat("X", &debugTranslationX, -1.0f, 1.0f);
    ImGui::SliderFloat("Y", &debugTranslationY, -1.0f, 1.0f);


    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    
    // Rendering
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


