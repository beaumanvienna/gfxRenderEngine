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
    
    static const int MAX_NUMBER_OF_CONTROLLERS = 4;
    static const int ANALOG_DEAD_ZONE = 15000;
    
    enum ControllerSticks
    {
        LEFT_STICK,
        RIGHT_STICK
    };
    
    enum ID
    {
        FIRST_CONTROLLER = 0,
        SECOND_CONTROLLER,
        THIRD_CONTROLLER,
        FOURTH_CONTROLLER
    };

    enum Axis
    {
        LEFT_STICK_HORIZONTAL = 0,
        LEFT_STICK_VERTICAL,
        RIGHT_STICK_HORIZONTAL,
        RIGHT_STICK_VERTICAL,
        LEFT_TRIGGER,
        RIGHT_TRIGGER
    };
    
    enum ControllerCode
    {    
        BUTTON_INVALID = -1,
        BUTTON_A,
        BUTTON_B,
        BUTTON_X,
        BUTTON_Y,
        BUTTON_BACK,
        BUTTON_GUIDE,
        BUTTON_START,
        BUTTON_LEFTSTICK,
        BUTTON_RIGHTSTICK,
        BUTTON_LEFTSHOULDER,
        BUTTON_RIGHTSHOULDER,
        BUTTON_DPAD_UP,
        BUTTON_DPAD_DOWN,
        BUTTON_DPAD_LEFT,
        BUTTON_DPAD_RIGHT,
        BUTTON_MAX
    };

    Controller(const std::string gamecontrollerdb = "", const std::string internaldb = "");
    ~Controller();
    
    bool Start();
    void OnUpdate();
    void Shutdown();
    
    void AddController(int indexID);
    void PrintJoyInfo(int indexID);
    void RemoveController(int instanceID);
    uint GetCount() const { return m_Controllers.size(); }
    SDL_GameController* GetGameController(int indexID) const;
    void CloseAllControllers();
    bool CheckControllerIsSupported(int indexID);
    bool CheckMapping(SDL_JoystickGUID guid, bool& mappingOK, std::string& name);
    bool FindGuidInFile(std::string& filename, char* text2match, int length, std::string* lineRet);
    bool FindGuidInFile(const char* path /* Linux */, int resourceID /* Windows */, const std::string& resourceClass /* Windows */, char* text2match, int length, std::string* lineRet);
    bool AddControllerToInternalDB(std::string entry);
    void RemoveDuplicatesInDB(void);
    
    void SetEventCallback(const EventCallbackFunction& callback);
    
private:

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
