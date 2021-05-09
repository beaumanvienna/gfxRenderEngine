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

#include "controller.h"

// --- Class Controller ---

Controller::Controller() 
    : m_Initialzed(false)
{
}

Controller::~Controller()
{
    CloseAllControllers();
}

bool Controller::Start()
{
    m_Initialzed = false;
    int i;

    if( SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0 )
    {
        LOG_WARN("Could not initialize SDL game controller subsystem");
    }
    else
    {
        
        std::string gamecontrollerdb = "resources/sdl/gamecontrollerdb.txt";
        if (( access( gamecontrollerdb.c_str(), F_OK ) == -1 ))
        {
            LOG_WARN("Could not find gamecontrollerdb.txt");
        }
        else
        {
            if( SDL_GameControllerAddMappingsFromFile(gamecontrollerdb.c_str()) == -1 )
            {
                LOG_WARN("Could not open gamecontrollerdb.txt");
            }
            else
            {
                LOG_INFO("SDL game controller subsystem initialized");
                m_Initialzed = true;
            }
        }
    }
    return m_Initialzed;
}

void Controller::Run()
{
    //Event handler
    SDL_Event event;
    
    //Handle events on queue
    while( SDL_PollEvent( &event ) != 0 )
    {
        // main event loop
        switch (event.type)
        {
            case SDL_JOYDEVICEADDED: 
                AddController(event.jdevice.which);
                break;
            case SDL_JOYDEVICEREMOVED: 
                RemoveController(event.jdevice.which);
                break;
            case SDL_JOYHATMOTION:
                break;
            case SDL_JOYAXISMOTION: 
                break;
            case SDL_JOYBUTTONDOWN: 
                break;
            case SDL_CONTROLLERBUTTONDOWN: 
                break;
        }
    }
}

void Controller::Shutdown()
{
    CloseAllControllers();
    m_Initialzed = false;
}

void Controller::PrintJoyInfo(int indexID)
{
    SDL_Joystick *joy = SDL_JoystickOpen(indexID);
    char guidStr[1024];
    const char* name = SDL_JoystickName(joy);
    int num_axes = SDL_JoystickNumAxes(joy);
    int num_buttons = SDL_JoystickNumButtons(joy);
    int num_hats = SDL_JoystickNumHats(joy);
    int num_balls = SDL_JoystickNumBalls(joy);
    int instance = SDL_JoystickInstanceID(joy);
    char *mapping;
    SDL_GameController *gameCtrl;
    
    SDL_JoystickGUID guid = SDL_JoystickGetGUID(joy);
    
    SDL_JoystickGetGUIDString(guid, guidStr, sizeof(guidStr));
    
    if (SDL_IsGameController(indexID)) 
    {
       // LOG_INFO("Index: {0}, Instance: {1}, Name: {2}, Number of axes: {3}, Number of buttons: {4}, Number of balls: {5}, compatible game controller", 
       //   indexID, instance, SDL_JoystickNameForIndex(indexID), SDL_JoystickNumAxes(joy), SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy));

        gameCtrl = SDL_GameControllerOpen(indexID);
        mapping = SDL_GameControllerMapping(gameCtrl);
        if (mapping) 
        {
            //LOG_INFO("Mapped as: {0}", mapping);
            SDL_free(mapping);
        }
    }
    else 
    {
        LOG_ERROR("Index: {0}, Instance: {1}, Name: {2}, Number of axes: {3}, Number of buttons: {4}, Number of balls: {5} ", 
          indexID, instance, SDL_JoystickNameForIndex(indexID), SDL_JoystickNumAxes(joy), SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy));
        LOG_ERROR("Index {0} is not a compatible controller", indexID);
    }
}

void Controller::AddController(int indexID)
{
    ControllerData controller;
    SDL_Joystick *joy;
    joy = SDL_JoystickOpen(indexID);
    
    if (joy) {
        PrintJoyInfo(indexID);
        //if (checkControllerIsSupported(indexID))
        {
            controller.m_IndexID = indexID;
            controller.m_Joystick = joy;
            controller.m_InstanceID = SDL_JoystickInstanceID(joy);
            controller.m_GameController = SDL_GameControllerFromInstanceID(controller.m_InstanceID);
            
            std::string joystickName = SDL_JoystickNameForIndex(indexID);
            transform(joystickName.begin(), joystickName.end(), joystickName.begin(),
                [](unsigned char c){ return tolower(c); });
            controller.m_Name = joystickName;
                    
            // mapping
            SDL_JoystickGUID guid = SDL_JoystickGetGUID(joy);
            //checkMapping(guid, controller.m_MappingOK,gDesignatedControllers[designation].name[device]);

            char *mappingString;
            mappingString = SDL_GameControllerMapping(controller.m_GameController);
            if (mappingString) 
            {
                std::string str = mappingString;
                SDL_free(mappingString);
                //remove guid
                str = str.substr(str.find(",")+1,str.length()-(str.find(",")+1));
                // extract name from db
                str = str.substr(0,str.find(","));
                
                transform(str.begin(), str.end(), str.begin(),
                    [](unsigned char c){ return tolower(c); });
                
                controller.m_NameDB = str;
                controller.m_MappingOK = true;
            }
            
            LOG_INFO("Adding controller index: {0}, instance: {1}, name: {2}, name in gamecontrollerdb.txt: {3}", 
                    controller.m_IndexID, controller.m_InstanceID, controller.m_Name, 
                    (controller.m_MappingOK ? controller.m_NameDB : "not found"));
                
            LOG_INFO("number of axes: {0}, number of buttons: {1}, number of balls: {2}, {3}", 
                    SDL_JoystickNumAxes(joy), SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy), 
                    (controller.m_MappingOK ? "mapping ok (compatible game controller)" : "mapping not ok"));
            
            LOG_INFO("active controllers: {0}", SDL_NumJoysticks());
            m_Controllers.push_back(controller);
            controller.m_Joystick = nullptr; // checked in destrcutor
        }
    } 
    else 
    {
        LOG_ERROR("Couldn't open Joystick {0}", indexID);
    }

    // Close if opened
    if (SDL_JoystickGetAttached(joy)) 
    {
        SDL_JoystickClose(joy);
    }
}

void Controller::RemoveController(int instanceID)
{
    for (auto controller = m_Controllers.begin(); controller != m_Controllers.end(); controller++) 
    {
        if (controller->m_InstanceID == instanceID) 
        {
            controller = m_Controllers.erase(controller);
            break;
        } 
    }
}

void Controller::CloseAllControllers()
{
    m_Controllers.clear();
}

Controller::ControllerData::ControllerData() :
            m_Joystick(nullptr),
            m_GameController(nullptr),
            m_InstanceID(-1),
            m_IndexID(-1),
            m_Name(""),
            m_NameDB(""),
            m_MappingOK(false)
{
    
}

Controller::ControllerData::~ControllerData()
{
    if (m_Joystick)
    {
        LOG_INFO("Removing controller index: {0}, instance: {1}, name: {2}, name in gamecontrollerdb.txt: {3}", m_IndexID, m_InstanceID, m_Name, m_NameDB);
        SDL_JoystickClose( m_Joystick );
    }
}
