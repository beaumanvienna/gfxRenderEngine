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

#include <iostream>

#include "common.h"
#include "core.h"
#include "screen.h"
#include "root.h"
#include "context.h"
#include "inputState.h"

SpriteSheet* SCREEN_ScreenManager::m_SpritesheetUI = nullptr;

SCREEN_ScreenManager::SCREEN_ScreenManager(std::shared_ptr<Renderer> renderer, SpriteSheet* spritesheetUI)
    : m_Renderer(renderer)
{
    m_SpritesheetUI = spritesheetUI;
    uiContext_ = new SCREEN_UIContext();
    dialogFinished_ = 0;
}

SCREEN_ScreenManager::~SCREEN_ScreenManager()
{
    shutdown();
}

//void SCREEN_ScreenManager::switchScreen(SCREEN_Screen *screen)
//{
//    if (!nextStack_.empty() && screen == nextStack_.front().screen)
//    {
//        LOG_CORE_WARN("Already switching to this screen");
//        return;
//    }
//    
//    if (!nextStack_.empty())
//    {
//        LOG_CORE_WARN("Already had a nextStack_! Asynchronous open while doing something? Deleting the new screen.");
//        delete screen;
//        return;
//    }
//    if (screen == nullptr)
//    {
//        LOG_CORE_WARN("Switching to a zero screen, this can't be good");
//    }
//    if (stack_.empty() || screen != stack_.back().screen) {
//        screen->setSCREEN_ScreenManager(this);
//        nextStack_.push_back({ screen, 0 });
//    }
//}
//

void SCREEN_ScreenManager::update()
{
    if (debugUI) LOG_CORE_INFO("********************* new frame *********************");
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    if (!nextStack_.empty())
    {
        switchToNext();
    }

    if (stack_.size())
    {
        stack_.back().screen->update();
    }
}

void SCREEN_ScreenManager::switchToNext()
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    if (nextStack_.empty())
    {
        LOG_CORE_WARN("switchToNext: No nextStack_!");
    }

    Layer temp = {nullptr, 0};
    if (!stack_.empty())
    {
        temp = stack_.back();
        stack_.pop_back();
    }
    stack_.push_back(nextStack_.front());
    if (temp.screen)
    {
        delete temp.screen;
    }
    SCREEN_UI::SetFocusedView(nullptr);

    for (size_t i = 1; i < nextStack_.size(); ++i)
    {
        stack_.push_back(nextStack_[i]);
    }
    nextStack_.clear();
}

bool SCREEN_ScreenManager::touch(const SCREEN_TouchInput &touch)
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    bool result = false;

    if (touch.flags & TOUCH_RELEASE_ALL)
    {
        for (auto &layer : stack_)
        {
            SCREEN_Screen *screen = layer.screen;
            result = layer.screen->touch(screen->transformTouch(touch));
        }
    }
    else if (!stack_.empty())
    {
        SCREEN_Screen *screen = stack_.back().screen;
        result = stack_.back().screen->touch(screen->transformTouch(touch));
    }
    return result;
}

bool SCREEN_ScreenManager::key(const SCREEN_KeyInput &key)
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    bool result = false;

    if (key.flags & KEY_UP)
    {
        for (auto &layer : stack_)
        {
            result = layer.screen->key(key);
        }
    }
    else if (!stack_.empty())
    {
        result = stack_.back().screen->key(key);
    }
    return result;
}

bool SCREEN_ScreenManager::axis(const SCREEN_AxisInput &axis)
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    bool result = false;
    // Send center axis to every screen layer.
    if (axis.value == 0)
    {
        for (auto &layer : stack_)
        {
            result = layer.screen->axis(axis);
        }
    }
    else if (!stack_.empty())
    {
        result = stack_.back().screen->axis(axis);
    }
    return result;
}

void SCREEN_ScreenManager::deviceLost()
{
    for (auto &iter : stack_)
        iter.screen->deviceLost();
}

void SCREEN_ScreenManager::deviceRestored()
{
    for (auto &iter : stack_)
        iter.screen->deviceRestored();
}

//void SCREEN_ScreenManager::resized()
//{
//    LOG_CORE_WARN("SCREEN_ScreenManager::resized(dp: %dx%d)", dp_xres, dp_yres);
//    std::lock_guard<std::recursive_mutex> guard(inputLock_);
//
//    for (auto iter = stack_.begin(); iter != stack_.end(); ++iter) {
//        iter->screen->resized();
//    }
//}
//
void SCREEN_ScreenManager::render()
{
    if (!stack_.empty())
    {
        switch (stack_.back().flags)
        {
            case LAYER_SIDEMENU:
            case LAYER_TRANSPARENT:
                if (stack_.size() == 1)
                {
                    LOG_CORE_WARN("Can't have sidemenu over nothing");
                    break;
                }
                else
                {
                    auto iter = stack_.end();
                    iter--;
                    iter--;
                    Layer backback = *iter;
    
                    backback.screen->preRender();
                    backback.screen->render();
                    stack_.back().screen->render();
                    if (postRenderCb_)
                        postRenderCb_(getUIContext(), postRenderUserdata_);
                    backback.screen->postRender();
                    break;
                }
            default:
                stack_.back().screen->preRender();
                stack_.back().screen->render();
                if (postRenderCb_)
                    postRenderCb_(getUIContext(), postRenderUserdata_);
                stack_.back().screen->postRender();
                break;
        }
    }
    else
    {
        LOG_CORE_WARN("No current screen!");
    }

    processFinishDialog();
}

//void SCREEN_ScreenManager::sendMessage(const char *msg, const char *value)
//{
//    if (!strcmp(msg, "recreateviews"))
//    {
//        RecreateAllViews();
//    }
//    if (!strcmp(msg, "lost_focus"))
//    {
//        SCREEN_TouchInput input;
//        input.flags = TOUCH_RELEASE_ALL;
//        input.timestamp = Engine::m_Engine->GetTime();
//        input.id = 0;
//        touch(input);
//    }
//    if (!stack_.empty())
//    {
//        stack_.back().screen->sendMessage(msg, value);
//    }
//}

SCREEN_Screen *SCREEN_ScreenManager::topScreen() const
{
    if (!stack_.empty())
    {
        return stack_.back().screen;
    }
    else
    {
        return 0;
    }
}

void SCREEN_ScreenManager::shutdown()
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    for (auto layer : stack_)
    {
        delete layer.screen;
    }
    stack_.clear();
    for (auto layer : nextStack_)
    {
        delete layer.screen;
    }
    nextStack_.clear();
}

void SCREEN_ScreenManager::push(SCREEN_Screen *screen, int layerFlags)
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    screen->setSCREEN_ScreenManager(this);
    if (screen->isTransparent())
    {
        layerFlags |= LAYER_TRANSPARENT;
    }

    lastFocusView.push(SCREEN_UI::GetFocusedView());
    SCREEN_UI::SetFocusedView(nullptr);
    SCREEN_TouchInput input;
    input.flags = TOUCH_RELEASE_ALL;
    input.timestamp = Engine::m_Engine->GetTime();
    input.id = 0;
    touch(input);

    Layer layer = {screen, layerFlags};
    if (nextStack_.empty())
    {
        stack_.push_back(layer);
    }
    else
    {
        nextStack_.push_back(layer);
    }
}

void SCREEN_ScreenManager::pop()
{
    std::lock_guard<std::recursive_mutex> guard(inputLock_);
    if (stack_.size())
    {
        delete stack_.back().screen;
        stack_.pop_back();
    }
    else
    {
        LOG_CORE_WARN("Can't pop when stack empty");
    }
}

void SCREEN_ScreenManager::RecreateAllViews()
{
    uiContext_->UIThemeInit();
    m_ViewsRecreated = true;
    for (auto it = stack_.begin(); it != stack_.end(); ++it)
    {
        it->screen->RecreateViews();
    }
}

void SCREEN_ScreenManager::finishDialog(SCREEN_Screen *dialog, DialogResult result)
{
    if (stack_.empty())
    {
        LOG_CORE_WARN("Must be in a dialog to finishDialog");
        return;
    }
    if (dialog != stack_.back().screen)
    {
        LOG_CORE_WARN("Wrong dialog being finished!");
        return;
    }
    dialog->onFinish(result);
    dialogFinished_ = dialog;
    dialogResult_ = result;
}

SCREEN_Screen *SCREEN_ScreenManager::dialogParent(const SCREEN_Screen *dialog) const
{
    for (size_t i = 1; i < stack_.size(); ++i)
    {
        if (stack_[i].screen == dialog)
        {
            return stack_[i - 1].screen;
        }
    }

    return nullptr;
}

void SCREEN_ScreenManager::processFinishDialog()
{
    if (dialogFinished_)
    {
        {
            std::lock_guard<std::recursive_mutex> guard(inputLock_);
            SCREEN_Screen *caller = dialogParent(dialogFinished_);
            for (size_t i = 0; i < stack_.size(); ++i)
            {
                if (stack_[i].screen == dialogFinished_) 
                {
                    stack_.erase(stack_.begin() + i);
                }
            }

            if (!caller)
            {
                LOG_CORE_WARN("Settings dialog finished");
            }
            else if (caller != topScreen())
            {
                LOG_CORE_WARN("Skipping non-top dialog when finishing dialog.");
            }
            else
            {
                caller->dialogFinished(dialogFinished_, dialogResult_);
            }
        }
        delete dialogFinished_;
        dialogFinished_ = nullptr;
        
        if (m_ViewsRecreated)
        {
            m_ViewsRecreated = false;
        }
        else
        {
            SCREEN_UI::SetFocusedView(lastFocusView.top());
        }
        lastFocusView.pop();
    }
}

