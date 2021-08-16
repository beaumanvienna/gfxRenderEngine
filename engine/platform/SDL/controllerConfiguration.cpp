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

#include "controllerConfiguration.h"
#include "input.h"

void ControllerConfiguration::Start(int controllerID)
{
    Reset();
    m_Running = true;
    m_ControllerID = controllerID;
    SetControllerConfText("press dpad up","(or use ENTER to skip this button)");
}

void ControllerConfiguration::Reset(void)
{
    m_Running = false;
    m_ControllerID = NO_CONTROLLER;
    
    for (int i = 0; i < BUTTON_MAX; i++)
    {
        m_ControllerButton[i]=STATE_CONF_SKIP_ITEM;
    }

    for (int i = 0; i < 4;i++)
    {
        m_Hat[i] = -1;
        m_HatValue[i] = -1;
        m_Axis[i] = -1;
        m_AxisValue[i] = false;
    }
    m_HatIterator = 0;
    m_AxisIterator = 0;
    m_SecondRun = -1;
    m_SecondRunHat = -1;
    m_SecondRunValue = -1;

    m_ConfigurationState = STATE_CONF_BUTTON_DPAD_UP;
    
    m_UpdateControllerText = false;
    m_Text1 = m_Text2 = "";
    m_MappingCreated = false;
}

void ControllerConfiguration::StatemachineConf(int cmd)
{
    if ((cmd==STATE_CONF_SKIP_ITEM) && (m_ConfigurationState > STATE_CONF_BUTTON_RIGHTSHOULDER))
    {
        StatemachineConfAxis(STATE_CONF_SKIP_ITEM,false);
        return;
    }
    
    if ((Input::GetActiveController() == m_ControllerID) || (cmd==STATE_CONF_SKIP_ITEM))
    {
        switch (m_ConfigurationState)
        {
            case STATE_CONF_BUTTON_DPAD_UP:
                if (m_SecondRun == -1)
                {
                    SetControllerConfText("press dpad up","(or use ENTER to skip this button)");
                    m_SecondRun = cmd;
                }
                else if (m_SecondRun == cmd)
                {
                    m_ControllerButton[m_ConfigurationState]=cmd;
                    m_ConfigurationState = STATE_CONF_BUTTON_DPAD_DOWN;
                    SetControllerConfText("press dpad down");
                    m_SecondRun = -1;
                }
                break;
            case STATE_CONF_BUTTON_DPAD_DOWN:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_DPAD_LEFT;
                SetControllerConfText("press dpad left");
                break;
            case STATE_CONF_BUTTON_DPAD_LEFT:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_DPAD_RIGHT;
                SetControllerConfText("press dpad right");
                break;
            case STATE_CONF_BUTTON_DPAD_RIGHT:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_A;
                SetControllerConfText("press south button (lower)");
                break;
            case STATE_CONF_BUTTON_A:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_B;
                SetControllerConfText("press east button (right)");
                break;
            case STATE_CONF_BUTTON_B:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_X;
                SetControllerConfText("press west button (left)");
                break;
            case STATE_CONF_BUTTON_X:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_Y;
                SetControllerConfText("press north button (upper)");
                break;
            case STATE_CONF_BUTTON_Y:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_LEFTSTICK;
                SetControllerConfText("press left stick button");
                break;
            case STATE_CONF_BUTTON_LEFTSTICK:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_RIGHTSTICK;
                SetControllerConfText("press right stick button");
                break;
            case STATE_CONF_BUTTON_RIGHTSTICK:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_LEFTSHOULDER;
                SetControllerConfText("press left front shoulder");
                break;
            case STATE_CONF_BUTTON_LEFTSHOULDER:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_RIGHTSHOULDER;
                SetControllerConfText("press right front shoulder");
                break;
            case STATE_CONF_BUTTON_RIGHTSHOULDER:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_BACK;
                SetControllerConfText("press select button");
                break;
            case STATE_CONF_BUTTON_BACK:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_START;
                SetControllerConfText("press start button");
                break;
            case STATE_CONF_BUTTON_START:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_BUTTON_GUIDE;
                SetControllerConfText("press guide button");
                break;
            case STATE_CONF_BUTTON_GUIDE:
                m_ControllerButton[m_ConfigurationState]=cmd;
                m_ConfigurationState = STATE_CONF_AXIS_LEFTSTICK_X;
                SetControllerConfText("twirl left stick");
                m_CountX=0;
                m_CountY=0;
                m_ValueX=-1;
                m_ValueY=-1;
                break;
            case STATE_CONF_AXIS_LEFTTRIGGER:
                m_ControllerButton[STATE_CONF_BUTTON_LEFTTRIGGER]=cmd;
                m_ConfigurationState = STATE_CONF_AXIS_RIGHTTRIGGER;
                SetControllerConfText("press right rear shoulder");
                break;
            case STATE_CONF_AXIS_RIGHTTRIGGER:
                m_ControllerButton[STATE_CONF_BUTTON_RIGHTTRIGGER]=cmd;
                SetMapping();
                break;
            default:
                (void) 0;
                break;
        }
    }
}

void ControllerConfiguration::StatemachineConfAxis(int cmd, bool negative)
{
    if ( (m_Running) && (m_ConfigurationState >= STATE_CONF_AXIS_LEFTSTICK_X) )
    {
        if ((Input::GetActiveController() == m_ControllerID)  || (cmd==STATE_CONF_SKIP_ITEM))
        {
            switch (m_ConfigurationState)
            {
                case STATE_CONF_AXIS_LEFTSTICK_X:
                case STATE_CONF_AXIS_LEFTSTICK_Y:
                    if (CheckAxis(cmd))
                    {
                        m_CountX=0;
                        m_CountY=0;
                        m_ValueX=-1;
                        m_ValueY=-1;

                        m_ConfigurationState = STATE_CONF_AXIS_RIGHTSTICK_X;
                        SetControllerConfText("twirl right stick");
                    }
                    break;
                case STATE_CONF_AXIS_RIGHTSTICK_X:
                case STATE_CONF_AXIS_RIGHTSTICK_Y:
                    if (cmd == STATE_CONF_SKIP_ITEM)
                    {
                        m_CountX=0;
                        m_CountY=0;
                        m_ValueX=-1;
                        m_ValueY=-1;
                        
                        m_ConfigurationState = STATE_CONF_AXIS_LEFTTRIGGER;
                        SetControllerConfText("press left rear shoulder");
                    }
                    else if ( (cmd != m_ControllerButton[STATE_CONF_AXIS_LEFTSTICK_X]) &&\
                            (cmd != m_ControllerButton[STATE_CONF_AXIS_LEFTSTICK_Y]))
                    {
                        
                        if (CheckAxis(cmd))
                        {
                            m_CountX=0;
                            m_CountY=0;
                            m_ValueX=-1;
                            m_ValueY=-1;
                            
                            m_ConfigurationState = STATE_CONF_AXIS_LEFTTRIGGER;
                            SetControllerConfText("press left rear shoulder");
                        }
                    }
                    break;
                case STATE_CONF_AXIS_LEFTTRIGGER:
                    if (cmd == STATE_CONF_SKIP_ITEM)
                    {
                        m_CountX=0;
                        m_ValueX=-1;
                          
                        m_ConfigurationState = STATE_CONF_AXIS_RIGHTTRIGGER;
                        SetControllerConfText("press right rear shoulder");
                    }
                    else if ( (cmd != m_ControllerButton[STATE_CONF_AXIS_RIGHTSTICK_X]) &&\
                            (cmd != m_ControllerButton[STATE_CONF_AXIS_RIGHTSTICK_Y]))
                    {
                        if (CheckTrigger(cmd))
                        {
                            m_CountX=0;
                            m_ValueX=-1;
                            
                            m_ConfigurationState = STATE_CONF_AXIS_RIGHTTRIGGER;
                            SetControllerConfText("press right rear shoulder");
                        }
                    }
                    
                    break;
                case STATE_CONF_AXIS_RIGHTTRIGGER:
                    if (cmd == STATE_CONF_SKIP_ITEM)
                    {
                        m_CountX=0;
                        m_ValueX=-1;
                        SetMapping();
                    }
                    else if (cmd != m_ControllerButton[STATE_CONF_AXIS_LEFTTRIGGER]) 
                    {
                        if (CheckTrigger(cmd))
                        {
                            m_CountX=0;
                            m_ValueX=-1;
                            SetMapping();
                        }
                    }
                    break;
                default:
                    (void) 0;
                    break;
            }
        }
    } else if ( (m_Running) && (m_ConfigurationState <= STATE_CONF_BUTTON_DPAD_RIGHT) )
    {
        if ((Input::GetActiveController() == m_ControllerID)  || (cmd==STATE_CONF_SKIP_ITEM))
        {
            m_Axis[m_AxisIterator] = cmd;
            m_AxisValue[m_AxisIterator] = negative;
            switch (m_ConfigurationState)
            {
                case STATE_CONF_BUTTON_DPAD_UP:
                    SetControllerConfText("press dpad down");
                    break;
                case STATE_CONF_BUTTON_DPAD_DOWN:
                    SetControllerConfText("press dpad left");
                    break;
                case STATE_CONF_BUTTON_DPAD_LEFT:
                    SetControllerConfText("press dpad right");
                    break;
                case STATE_CONF_BUTTON_DPAD_RIGHT:
                    SetControllerConfText("press south button (lower)");
                    break;
                default:
                    (void) 0;
                    break;
            }
            m_ConfigurationState++;
            m_AxisIterator++;
        }
    }
}

bool ControllerConfiguration::CheckAxis(int cmd)
{
    if (cmd==STATE_CONF_SKIP_ITEM) return true;
    
    bool ok = false;
    
    if ( (m_CountX > 10) && (m_CountY>10) )
    {
        m_ControllerButton[m_ConfigurationState]=m_ValueX;
        m_ControllerButton[m_ConfigurationState+1]=m_ValueY;
        ok = true;
    }
    
    if ( (m_ValueX!=-1) && (m_ValueY!=-1) )
    {
        if (m_ValueX == cmd) m_CountX++;
        if (m_ValueY == cmd) m_CountY++;
    }
    
    if ( (m_ValueX!=-1) && (m_ValueY==-1) )
    {
        if (m_ValueX > cmd)
        {
            m_ValueY = m_ValueX;
            m_ValueX = cmd;
        }
        else if (m_ValueX != cmd)
        {
            m_ValueY = cmd;
        }
    }
    
    if ( (m_ValueX==-1) && (m_ValueY==-1) )
    {
        m_ValueX=cmd;
    }   

    return ok;
}

bool ControllerConfiguration::CheckTrigger(int cmd)
{
    if (cmd==STATE_CONF_SKIP_ITEM) return true;
    bool ok = false;

    if (m_CountX > 10)
    {
        m_ControllerButton[m_ConfigurationState]=m_ValueX;
        ok = true;
    }

    if (m_ValueX!=-1)
    {
        if (m_ValueX == cmd) m_CountX++;
    }
    
    if (m_ValueX==-1)
    {
        m_ValueX=cmd;
    }   

    return ok;
}

void ControllerConfiguration::StatemachineConfHat(int hat, int value)
{

    m_Hat[m_HatIterator] = hat;
    m_HatValue[m_HatIterator] = value;

    if (Input::GetActiveController() == m_ControllerID)
    {

        switch (m_ConfigurationState)
        {
            case STATE_CONF_BUTTON_DPAD_UP:            
                if ( (m_SecondRunHat == -1) && (m_SecondRunValue == -1) )
                {
                    SetControllerConfText("press dpad up again");
                    m_SecondRunHat = hat;
                    m_SecondRunValue = value;
                }
                else if ( (m_SecondRunHat == hat) && (m_SecondRunValue == value) )
                {
                    m_HatIterator++;
                    m_ConfigurationState = STATE_CONF_BUTTON_DPAD_DOWN;
                    SetControllerConfText("press dpad down");
                    m_SecondRunHat = -1;
                    m_SecondRunValue = -1;
                }
                break;
            case STATE_CONF_BUTTON_DPAD_DOWN:
                m_ConfigurationState = STATE_CONF_BUTTON_DPAD_LEFT;
                SetControllerConfText("press dpad left");
                m_HatIterator++;
                break;
            case STATE_CONF_BUTTON_DPAD_LEFT: 
                m_ConfigurationState = STATE_CONF_BUTTON_DPAD_RIGHT;
                SetControllerConfText("press dpad right");
                m_HatIterator++;
                break;
            case STATE_CONF_BUTTON_DPAD_RIGHT: 
                m_ConfigurationState = STATE_CONF_BUTTON_A;
                SetControllerConfText("press south button (lower)");
                m_HatIterator++;
                break;
            default:
                (void) 0;
                break;
        }
    }
}
void ControllerConfiguration::SetControllerConfText(std::string text1, std::string text2)
{
    m_Text1 = text1;
    if (text2 != "") m_Text2 = text2;
    
    m_UpdateControllerText = true;
}

void ControllerConfiguration::SetMapping(void)
{
    m_MappingCreated = true;
    SetControllerConfText("Start controller setup (" + std::to_string(m_ControllerID + 1) + ")");
    LOG_CORE_INFO("Mapping created!");
}
