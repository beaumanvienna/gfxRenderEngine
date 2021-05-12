/* Controller Copyright (c) 2021 Controller Development Team 
   https://github.com/beaumanvienna/gfxRenderController

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

#include <list>
#include <SDL.h>
#include <memory>
#include <vector>

#include "engine.h"
#include "platform.h"
#include "event.h"

class Controller
{
    
public:

    Controller(const std::string gamecontrollerdb = "", const std::string internaldb = "");
    ~Controller();
    
    bool Start();
    void OnUpdate();
    void Shutdown();
    
    void AddController(int indexID);
    void PrintJoyInfo(int indexID);
    void RemoveController(int instanceID);
    void CloseAllControllers();
    bool CheckControllerIsSupported(int indexID);
    bool CheckMapping(SDL_JoystickGUID guid, bool& mappingOK, std::string& name);
    bool FindGuidInFile(std::string& filename, char* text2match, int length, std::string* lineRet);
    bool AddControllerToInternalDB(std::string entry);
    void RemoveDuplicatesInDB(void);
    
    void SetEventCallback(const EventCallbackFunction& callback);
    
private:

    const int ANALOG_DEAD_ZONE = 0;

    bool m_Initialzed;
    EventCallbackFunction m_EventCallback;
    std::string m_Gamecontrollerdb, m_InternalDB;

    class ControllerData
    {
    public:
        int m_InstanceID;
        int m_IndexID;
        SDL_Joystick* m_Joystick;
        SDL_GameController* m_GameController;
        std::string m_Name;
        std::string m_NameDB;
        bool m_MappingOK;
        
        ControllerData();
        ~ControllerData();
    };
    
    std::list<ControllerData> m_Controllers;
    std::vector<int> m_InstanceToIndex;

};
