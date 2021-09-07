/******************************************************************************/
/* Mednafen - Multi-system Emulator                                           */
/******************************************************************************/
/* Joystick_SDL.cpp:
**  Copyright (C) 2012-2018 Mednafen Team
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "main.h"
#include "input.h"
#include "Joystick.h"
#include "Joystick_SDL.h"
#include <mednafen/hash/md5.h>

#define MAX_DEVICES_PER_CONTROLLER 1 
#define MAX_GAMEPADS 2

    typedef SDL_Joystick* pSDL_Joystick;
    typedef SDL_GameController* pSDL_GameController;
    // controllers detected by SDL 
    // will be assigned a slot
    // (designated controller 0, controller 1)
    typedef struct DesignatedControllers { 
        pSDL_Joystick joy[MAX_DEVICES_PER_CONTROLLER];
        pSDL_GameController gameCtrl[MAX_DEVICES_PER_CONTROLLER];
        int instance[MAX_DEVICES_PER_CONTROLLER];
        int index[MAX_DEVICES_PER_CONTROLLER];
        std::string name[MAX_DEVICES_PER_CONTROLLER];
        std::string nameDB[MAX_DEVICES_PER_CONTROLLER];
        bool mappingOKDevice[MAX_DEVICES_PER_CONTROLLER];
        bool mappingOK;
        int controllerType;
        int numberOfDevices;
    } T_DesignatedControllers;
    
extern T_DesignatedControllers gDesignatedControllers[MAX_GAMEPADS];

#include <SDL.h>
#ifndef _WIN32
#warning "JC: modified"
#endif // !_WIN32

extern int NeedExitNow;
class Joystick_SDL : public Joystick
{
 public:

 Joystick_SDL(unsigned index) MDFN_COLD;
 ~Joystick_SDL() MDFN_COLD;

 void UpdateInternal(void);

 virtual unsigned HatToButtonCompat(unsigned hat);

 private:
 SDL_Joystick *sdl_joy;
 unsigned sdl_num_axes;
 unsigned sdl_num_hats;
 unsigned sdl_num_balls;
 unsigned sdl_num_buttons;
#ifndef _WIN32
    #warning "jc: modified"
#endif // !_WIN32

 const char* sdl_name;
 SDL_GameController* sdl_game_controller;
};

unsigned Joystick_SDL::HatToButtonCompat(unsigned hat)
{
 return(sdl_num_buttons + (hat * 4));
}

Joystick_SDL::Joystick_SDL(unsigned index) : sdl_joy(NULL)
{
#ifndef _WIN32
    #warning "jc: modified"
#endif // !_WIN32
 sdl_joy = gDesignatedControllers[index].joy[0];
 sdl_game_controller = gDesignatedControllers[index].gameCtrl[0];
 //sdl_joy = SDL_JoystickOpen(index);
 
 if(sdl_joy == NULL)
 {
  throw MDFN_Error(0, "SDL_JoystickOpen(%u) failed: %s", index, SDL_GetError());
 }

 try
 {
  name = SDL_JoystickName(sdl_joy);
#ifndef _WIN32
  #warning "jc: modified"
#endif // !_WIN32
  sdl_name = SDL_JoystickName(sdl_joy);
  
  sdl_num_axes = SDL_JoystickNumAxes(sdl_joy);
  sdl_num_balls = SDL_JoystickNumBalls(sdl_joy);
  sdl_num_buttons = SDL_JoystickNumButtons(sdl_joy);
  sdl_num_hats = SDL_JoystickNumHats(sdl_joy);

  Calc09xID(sdl_num_axes, sdl_num_balls, sdl_num_hats, sdl_num_buttons);
  {
   //SDL_JoystickGUID guid = SDL_JoystickGetGUID(sdl_joy);
   //memcpy(&id[0], guid.data, 16);
   //
   // Don't use SDL's GUID, as it's just equivalent to part of the joystick name on many platforms.
   // 
#ifndef _WIN32
      #warning "jc: modified"
#endif // !_WIN32
   // all sdl game controller look the same and get the ID "bad food beef babe"
   //  0xbaadf00d00000000beefbabe00000000
   // (actual mapping is done in marley)
   unsigned long long l[2] = {0x0DF0ADBA, 0xBEBAEFBE};
   memcpy(&id[0], l, 16);
  /*
   md5_context h;
   uint8 d[16];

   h.starts();
   h.update((const uint8*)name.data(), name.size());
   h.finish(d);
   memcpy(&id[0], d, 8);

   MDFN_en16msb(&id[ 8], sdl_num_axes);
   MDFN_en16msb(&id[10], sdl_num_buttons);
   MDFN_en16msb(&id[12], sdl_num_hats);
   MDFN_en16msb(&id[14], sdl_num_balls);
   */
  }
  
  num_axes = sdl_num_axes;
  num_rel_axes = sdl_num_balls * 2;
  //num_buttons = sdl_num_buttons + (sdl_num_hats * 4);
  num_buttons = SDL_CONTROLLER_BUTTON_MAX;

  axis_state.resize(num_axes);
  rel_axis_state.resize(num_rel_axes);
  button_state.resize(num_buttons);
 }
 catch(...)
 {
  if(sdl_joy)
   SDL_JoystickClose(sdl_joy);

  throw;
 }
}

Joystick_SDL::~Joystick_SDL()
{
 if(sdl_joy)
 {
#ifndef _WIN32
     #warning "jc: modified"
#endif // !_WIN32
  //SDL_JoystickClose(sdl_joy);
  sdl_joy = NULL;
 }
}

void Joystick_SDL::UpdateInternal(void)
{

 for(unsigned i = 0; i < sdl_num_axes; i++)
 {
  axis_state[i] = SDL_JoystickGetAxis(sdl_joy, i);
  if(axis_state[i] < -32767)
   axis_state[i] = -32767;
 }

 for(unsigned i = 0; i < sdl_num_balls; i++)
 {
  int dx, dy;

  SDL_JoystickGetBall(sdl_joy, i, &dx, &dy);

  rel_axis_state[i * 2 + 0] = dx;
  rel_axis_state[i * 2 + 1] = dy;
 }

#ifndef _WIN32
 #warning "jc: modified"
#endif // !_WIN32
 //unsigned int n= sdl_num_buttons + 4*sdl_num_hats + 2*sdl_num_axes;
 for(unsigned i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
 {
  button_state[i] = SDL_GameControllerGetButton(sdl_game_controller, (SDL_GameControllerButton)i);
 }
#ifndef _WIN32
 #warning "jc: modified"
#endif // !_WIN32
 if (button_state[SDL_CONTROLLER_BUTTON_GUIDE] == 1) NeedExitNow = 1;
 /*
 for(unsigned i = 0; i < sdl_num_buttons; i++)
 {
  button_state[i] = SDL_JoystickGetButton(sdl_joy, i);
 }
 
 for(unsigned i = 0; i < sdl_num_hats; i++)
 {
  uint8 hs = SDL_JoystickGetHat(sdl_joy, i);

  button_state[sdl_num_buttons + (i * 4) + 0] = (bool)(hs & SDL_HAT_UP);
  button_state[sdl_num_buttons + (i * 4) + 1] = (bool)(hs & SDL_HAT_RIGHT);
  button_state[sdl_num_buttons + (i * 4) + 2] = (bool)(hs & SDL_HAT_DOWN);
  button_state[sdl_num_buttons + (i * 4) + 3] = (bool)(hs & SDL_HAT_LEFT);
 }
 */
}

class JoystickDriver_SDL : public JoystickDriver
{
 public:

 JoystickDriver_SDL();
 virtual ~JoystickDriver_SDL();

 virtual unsigned NumJoysticks();                       // Cached internally on JoystickDriver instantiation.
 virtual Joystick *GetJoystick(unsigned index);
 virtual void UpdateJoysticks(void);

 private:
 std::vector<Joystick_SDL *> joys;
};


JoystickDriver_SDL::JoystickDriver_SDL()
{
#ifndef _WIN32
    #warning "jc: modified"
#endif // !_WIN32
 
 for(int n = 0; n < MAX_GAMEPADS; n++)
 {
   if (gDesignatedControllers[n].joy[0] != NULL)
   {
     Joystick_SDL *jsdl = new Joystick_SDL(n);
     joys.push_back(jsdl);
   }
 }
 /*
 SDL_InitSubSystem(SDL_INIT_JOYSTICK);

 for(int n = 0; n < SDL_NumJoysticks(); n++)
 {
  try
  {
   Joystick_SDL *jsdl = new Joystick_SDL(n);
   joys.push_back(jsdl);
  }
  catch(std::exception &e)
  {
   MDFND_OutputNotice(MDFN_NOTICE_ERROR, e.what());
  }
 }
 */
}

JoystickDriver_SDL::~JoystickDriver_SDL()
{
    
 for(unsigned int n = 0; n < joys.size(); n++)
 {
  delete joys[n];
 }

#ifndef _WIN32
    #warning "jc: modified"
#endif // !_WIN32
 //SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

}

unsigned JoystickDriver_SDL::NumJoysticks(void)
{
 return joys.size();
}

Joystick *JoystickDriver_SDL::GetJoystick(unsigned index)
{
 return joys[index];
}

void JoystickDriver_SDL::UpdateJoysticks(void)
{
 SDL_JoystickUpdate();

 for(unsigned int n = 0; n < joys.size(); n++)
 {
  joys[n]->UpdateInternal();
 }
}

JoystickDriver *JoystickDriver_SDL_New(void)
{
 return new JoystickDriver_SDL();
}
