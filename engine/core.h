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

#include <iostream>
#include <memory>
#include <functional>

#include "rendererAPI.h"
#include "renderer.h"
#include "window.h"
#include "event.h"
#include "controller.h"
#include "layerStack.h"
#include "graphicsContext.h"
#include "timestep.h"
#include "timer.h"
#include "settings.h"

class Engine
{
public:

    static constexpr bool SWITCH_OFF_COMPUTER = true;

public:

    Engine(int argc, char** argv);
    ~Engine();
    
    bool Start(RendererAPI::API api);
    void OnUpdate();
    void OnRender();
    void Shutdown(bool switchOffComputer = false);
    void Quit();
    void OnEvent(Event& event);
    
    void  SetWindowAspectRatio(); 
    float GetWindowAspectRatio() const { return m_Window->GetWindowAspectRatio(); }
    float GetWindowScale() const { return m_Window->GetWindowScale(); }
    float GetWindowWidth() const { return m_Window->GetWidth(); }
    float GetWindowHeight() const { return m_Window->GetHeight(); }
    float GetContextWidth() const { return 1920.0f; }
    float GetContextHeight() const { return GetContextWidth() / GetWindowAspectRatio(); }
    float GetScaleImguiWidgets() const { return m_ScaleImguiWidgets; }
    void* GetWindow() const { return m_Window->GetWindow(); }
    void ToggleFullscreen();
    bool IsFullscreen() const { return m_Fullscreen; }
    
    bool IsRunning() const { return m_Running; }
    bool IsPaused() const { return m_Paused; }
    double GetTime() const { return m_Window->GetTime(); }
    Timestep GetTimestep() const { return m_Timestep; }
    
    void SetAppEventCallback(EventCallbackFunction eventCallback);
    
    void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
    void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
    void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }
    void PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }
    
    void EnableMousePointer() { m_Window->EnableMousePointer(); }
    void DisableMousePointer() { m_Window->DisableMousePointer(); }
    
    void SetRenderer(std::shared_ptr<Renderer>& renderer) { m_Renderer = renderer; }
    std::shared_ptr<Renderer>& GetRenderer() { return m_Renderer; }
    
    static Engine* m_Engine;
    
private:
    static void SignalHandler(int signal);
    
private:

    bool m_Running, m_Paused, m_Fullscreen, m_SwitchOffComputer;
    std::unique_ptr<Window> m_Window;
    std::shared_ptr<GraphicsContext>(m_GraphicsContext);
    float m_ScaleImguiWidgets;
    Controller m_Controller;
    LayerStack m_LayerStack;
    
    EventCallbackFunction m_AppEventCallback;
    Timestep m_Timestep;
    float m_TimeLastFrame;
    Timer m_DisableMousePointerTimer;
    
    std::shared_ptr<Renderer> m_Renderer;
    Settings m_Settings;
    
};
