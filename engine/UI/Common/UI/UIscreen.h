/* Copyright (c) 2013-2020 PPSSPP project
   https://github.com/hrydgard/ppsspp/blob/master/LICENSE.TXT
   
   Engine Copyright (c) 2021 Engine Development Team 
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

#include "screen.h"
#include "view.h"
#include "viewGroup.h"
#include "glm.hpp"

class SCREEN_I18NCategory;
namespace SCREEN_Draw {
    class SCREEN_DrawContext;
}

class SCREEN_UIScreen : public SCREEN_Screen 
{
public:
    SCREEN_UIScreen();
    ~SCREEN_UIScreen();

    void update() override;
    void preRender() override;
    void render() override;
    void postRender() override;
    void deviceLost() override;
    void deviceRestored() override;

    bool touch(const SCREEN_TouchInput &touch) override;
    bool key(const SCREEN_KeyInput &touch) override;
    bool axis(const SCREEN_AxisInput &touch) override;

    SCREEN_TouchInput transformTouch(const SCREEN_TouchInput &touch) override;

    virtual void TriggerFinish(DialogResult result);

    // Some useful default event handlers
    SCREEN_UI::EventReturn OnOK(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn OnCancel(SCREEN_UI::EventParams &e);
    SCREEN_UI::EventReturn OnBack(SCREEN_UI::EventParams &e);

protected:
    virtual void CreateViews() = 0;
    virtual void DrawBackground(SCREEN_UIContext &dc) {}

    virtual void RecreateViews() override { recreateViews_ = true; }

    SCREEN_UI::ViewGroup *root_ = nullptr;
    glm::vec3 translation_ = glm::vec3(0.0f);
    glm::vec3 scale_ = glm::vec3(1.0f);
    float alpha_ = 1.0f;
    bool ignoreInsets_ = false;

private:
    void DoRecreateViews();

    bool recreateViews_ = true;
};

class SCREEN_UIDialogScreen : public SCREEN_UIScreen
{
public:
    SCREEN_UIDialogScreen() 
        : SCREEN_UIScreen(), finished_(false) 
    {}
    
    bool key(const SCREEN_KeyInput &key) override;
    void sendMessage(const char *msg, const char *value) override;

private:
    bool finished_;
};
