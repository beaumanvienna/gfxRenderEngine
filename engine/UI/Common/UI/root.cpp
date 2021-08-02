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

#include <mutex>
#include <set>
#include <deque>

#include "core.h"
#include "common.h"
#include "root.h"
#include "geom2d.h"
#include "inputState.h"
#include "viewGroup.h"
#include "context.h"
#include "input.h"

namespace SCREEN_UI
{
    static std::mutex focusLock;
    static std::vector<int> focusMoves;
    extern bool focusForced;
   
    static View* focusedView = nullptr;
    static bool focusMovementEnabled;
    bool focusForced;
    static std::mutex eventMutex_;

    struct DispatchQueueItem
    {
        Event *e;
        EventParams params;
    };
   
    std::deque<DispatchQueueItem> g_dispatchQueue;
   
    void EventTriggered(Event *e, EventParams params)
    {
        DispatchQueueItem item;
        item.e = e;
        item.params = params;
   
        std::unique_lock<std::mutex> guard(eventMutex_);
        g_dispatchQueue.push_front(item);
    }
   
    void DispatchEvents()
    {
        while (true) {
            DispatchQueueItem item;
            {
                std::unique_lock<std::mutex> guard(eventMutex_);
                if (g_dispatchQueue.empty())
                    break;
                item = g_dispatchQueue.back();
                g_dispatchQueue.pop_back();
            }
            if (item.e) {
                item.e->Dispatch(item.params);
            }
        }
    }
   
    void RemoveQueuedEventsByView(View *view)
    {
        std::unique_lock<std::mutex> guard(eventMutex_);
        for (auto it = g_dispatchQueue.begin(); it != g_dispatchQueue.end(); )
        {
            if (it->params.v == view)
            {
                it = g_dispatchQueue.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
   
    void RemoveQueuedEventsByEvent(Event *event)
    {
        std::unique_lock<std::mutex> guard(eventMutex_);
        for (auto it = g_dispatchQueue.begin(); it != g_dispatchQueue.end(); )
        {
            if (it->e == event)
            {
                it = g_dispatchQueue.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
   
    View *GetFocusedView()
    {
        return focusedView;
    }
   
    void SetFocusedView(View *view, bool force)
    {
        if (focusedView)
        {
            focusedView->FocusChanged(FF_LOSTFOCUS);
        }
        focusedView = view;
        if (focusedView)
        {
            focusedView->FocusChanged(FF_GOTFOCUS);
            if (force)
            {
                focusForced = true;
            }
        }
    }
   
    void EnableFocusMovement(bool enable)
    {
        focusMovementEnabled = enable;
        if (!enable)
        {
            if (focusedView)
            {
                focusedView->FocusChanged(FF_LOSTFOCUS);
            }
            focusedView = nullptr;
        }
    }
   
    bool IsFocusMovementEnabled()
    {
        return focusMovementEnabled;
    }
   
    void LayoutViewHierarchy(const SCREEN_UIContext &dc, ViewGroup *root, bool ignoreInsets)
    {
        if (!root)
        {
            LOG_CORE_ERROR("Tried to layout a view hierarchy from a zero pointer root");
            return;
        }
   
        Bounds rootBounds = ignoreInsets ? dc.GetBounds() : dc.GetLayoutBounds();
   
        MeasureSpec horiz(EXACTLY, rootBounds.w);
        MeasureSpec vert(EXACTLY, rootBounds.h);

        root->Measure(dc, horiz, vert);       
        root->SetBounds(rootBounds);
        root->Layout();
    }
   
    void MoveFocus(ViewGroup *root, FocusDirection direction)
    {
        if (!GetFocusedView())
        {
            root->SetFocus();
            return;
        }
        NeighborResult neigh(0, 0);
        neigh = root->FindNeighbor(GetFocusedView(), direction, neigh);
   
        if (neigh.view)
        {
            neigh.view->SetFocus();
            root->SubviewFocused(neigh.view);
        }
    }
   
//    void SetSoundEnabled(bool enabled)
//    {
//        soundEnabled = enabled;
//    }
//   
//    void SetSoundCallback(std::function<void(SCREEN_UISound)> func)
//    {
//        soundCallback = func;
//    }
//   
//    void PlayUISound(SCREEN_UISound sound)
//    {
//        if (soundEnabled && soundCallback)
//        {
//            soundCallback(sound);
//        }
//    }

    struct HeldKey
    {
        int key;
        int deviceId;
        double triggerTime;
   
        bool operator <(const HeldKey &other) const
        {
            if (key < other.key) return true;
            return false;
        }
        bool operator ==(const HeldKey &other) const { return key == other.key; }
    };

    static std::set<HeldKey> heldKeys;
   
    const double repeatDelay = 15 * (1.0 / 60.0f);
    const double repeatInterval = 5 * (1.0 / 60.0f);
   
    bool KeyEvent(const SCREEN_KeyInput &key, ViewGroup *root)
    {
        bool retval = false;

        if ((key.flags & (KEY_DOWN | KEY_IS_REPEAT)) == KEY_DOWN)
        {
            HeldKey hk;
            hk.key = key.keyCode;
            hk.deviceId = key.deviceId;
            hk.triggerTime = Engine::m_Engine->GetTime() + repeatDelay;

            if (heldKeys.find(hk) != heldKeys.end())
            {
                return false;
            }
   
            heldKeys.insert(hk);
            std::lock_guard<std::mutex> lock(focusLock);
            focusMoves.push_back(key.keyCode);
            retval = true;
        }
        if (key.flags & KEY_UP)
        {
            if (!heldKeys.empty())
            {
                HeldKey hk;
                hk.key = key.keyCode;
                hk.deviceId = key.deviceId;
                hk.triggerTime = 0.0;
                if (heldKeys.find(hk) != heldKeys.end())
                {
                    heldKeys.erase(hk);
                    retval = true;
                }
            }
        }
   
        retval = root->Key(key);
   
        switch (key.keyCode)
        {
            case NKCODE_VOLUME_DOWN:
            case NKCODE_VOLUME_UP:
            case NKCODE_VOLUME_MUTE:
                retval = false;
                break;
        }
   
        return retval;
    }
   
    static void ProcessHeldKeys(ViewGroup *root)
    {
        double now = Engine::m_Engine->GetTime();
   
    restart:
   
        for (std::set<HeldKey>::iterator iter = heldKeys.begin(); iter != heldKeys.end(); ++iter)
        {
            if (iter->triggerTime < now)
            {
                SCREEN_KeyInput key;
                key.keyCode = iter->key;
                key.deviceId = iter->deviceId;
                key.flags = KEY_DOWN;
                KeyEvent(key, root);
   
                std::lock_guard<std::mutex> lock(focusLock);
                focusMoves.push_back(key.keyCode);
   
                HeldKey hk = *iter;
                heldKeys.erase(hk);
                hk.triggerTime = now + repeatInterval;
                heldKeys.insert(hk);
                goto restart;
            }
        }
    }
   
    bool TouchEvent(const SCREEN_TouchInput &touch, ViewGroup *root)
    {
        focusForced = false;
        root->Touch(touch);
        if ((touch.flags & TOUCH_DOWN) && !focusForced)
        {
            EnableFocusMovement(false);
        }
        return true;
    }
   
    bool AxisEvent(const SCREEN_AxisInput &axis, ViewGroup *root)
    {
        enum class SCREEN_DirState
        {
            NONE = 0,
            POS = 1,
            NEG = 2,
        };
        struct PrevState
        {
            PrevState() : x(SCREEN_DirState::NONE), y(SCREEN_DirState::NONE)
            {
            }
   
            SCREEN_DirState x;
            SCREEN_DirState y;
        };
        struct StateKey
        {
            int deviceId;
            int axisId;
   
            bool operator <(const StateKey &other) const
            {
                return std::tie(deviceId, axisId) < std::tie(other.deviceId, other.axisId);
            }
        };
        static std::map<StateKey, PrevState> state;
        StateKey stateKey{ axis.deviceId, axis.axisId };

        auto GenerateKeyFromAxis = [&](SCREEN_DirState old, SCREEN_DirState cur, int neg_key, int pos_key)
        {
            if (old == cur)
            {
                return;
            }
            if (old == SCREEN_DirState::POS)
            {
                KeyEvent(SCREEN_KeyInput{ DEVICE_ID_KEYBOARD, pos_key, KEY_UP }, root);
            }
            else if (old == SCREEN_DirState::NEG)
            {
                KeyEvent(SCREEN_KeyInput{ DEVICE_ID_KEYBOARD, neg_key, KEY_UP }, root);
            }
            if (cur == SCREEN_DirState::POS)
            {
                KeyEvent(SCREEN_KeyInput{ DEVICE_ID_KEYBOARD, pos_key, KEY_DOWN }, root);
            }
            else if (cur == SCREEN_DirState::NEG)
            {
                KeyEvent(SCREEN_KeyInput{ DEVICE_ID_KEYBOARD, neg_key, KEY_DOWN }, root);
            }
        };

        const float THRESHOLD = 0.75;
        
        switch (axis.deviceId)
        {
            case DEVICE_ID_PAD_0:
            case DEVICE_ID_PAD_1:
            case DEVICE_ID_PAD_2:
            case DEVICE_ID_PAD_3:
            case DEVICE_ID_X360_0:
            case DEVICE_ID_X360_1:
            case DEVICE_ID_X360_2:
            case DEVICE_ID_X360_3:
            {
                PrevState &old = state[stateKey];
                SCREEN_DirState dir = SCREEN_DirState::NONE;
                if (axis.value < -THRESHOLD)
                {
                    dir = SCREEN_DirState::NEG;
                }
                else if (axis.value > THRESHOLD)
                {
                    dir = SCREEN_DirState::POS;
                }

                if (axis.axisId == Controller::RIGHT_STICK_HORIZONTAL)
                {
                    GenerateKeyFromAxis(old.x, dir, Controller::BUTTON_DPAD_LEFT, Controller::BUTTON_DPAD_RIGHT);
                    old.x = dir;
                }
                else if (axis.axisId == Controller::RIGHT_STICK_VERTICAL)
                {

                    GenerateKeyFromAxis(old.y, dir, Controller::BUTTON_DPAD_DOWN, Controller::BUTTON_DPAD_UP);
                    old.y = dir;
                }
                break;
            }
        }

        root->Axis(axis);
        return true;
    }

    void UpdateViewHierarchy(ViewGroup *root)
    {
        ProcessHeldKeys(root);
  
        if (!root)
        {
            LOG_CORE_WARN("Tried to update a view hierarchy from a zero pointer root");
            return;
        }
      
        if (focusMoves.size())
        {
            std::lock_guard<std::mutex> lock(focusLock);
            EnableFocusMovement(true);
            if (!GetFocusedView()) {
                View *defaultView = root->GetDefaultFocusView();
                if (defaultView && defaultView->GetVisibility() == V_VISIBLE)
                {
                    root->GetDefaultFocusView()->SetFocus();
                }
                else
                {
                    root->SetFocus();
                }
                root->SubviewFocused(GetFocusedView());
            }
            else
            {
                for (size_t i = 0; i < focusMoves.size(); i++)
                {
                    switch (focusMoves[i])
                    {
                        case Controller::BUTTON_DPAD_LEFT:
                        case ENGINE_KEY_LEFT:
                            MoveFocus(root, FOCUS_LEFT);
                            break;
                        case ENGINE_KEY_RIGHT:
                        case Controller::BUTTON_DPAD_RIGHT:
                            MoveFocus(root, FOCUS_RIGHT);
                            break;
                        case ENGINE_KEY_UP:
                        case Controller::BUTTON_DPAD_UP:
                            MoveFocus(root, FOCUS_UP);
                            break;
                        case ENGINE_KEY_DOWN:
                        case Controller::BUTTON_DPAD_DOWN:
                            MoveFocus(root, FOCUS_DOWN);
                            break;
                        default:
                            break;
                    }
                }
            }
            focusMoves.clear();
        }
  
        root->Update();
        DispatchEvents();
    }
}
