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

#include "engine.h"
#include "UIscreen.h"

class MainScreen : public SCREEN_UIDialogScreen 
{
public:
    MainScreen();
    virtual ~MainScreen();
    bool key(const SCREEN_KeyInput &key) override;
    void update() override;
    void onFinish(DialogResult result) override;
    std::string tag() const override { return "main screen"; }

protected:
    void CreateViews() override;
    
    SCREEN_UI::EventReturn settingsClick(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn offClick(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn offHold(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn HomeClick(SCREEN_UI::EventParams &e);

private:

    SCREEN_UI::Choice* offButton;

};

