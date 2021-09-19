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

//#include "common.h"
#include "core.h"
#include "marley/UI/browser/ROMbrowser.h"
#include "marley/UI/browser/ROMbutton.h"
#include "context.h"
#include "drawBuffer.h"
#include "i18n.h"
#include "root.h"

#include <dirent.h>
#include <fstream>

namespace MarleyApp
{
    bool gGamesFound = false;
    bool stopSearching = false;
    std::vector<std::string> gFileTypes = {"smc","iso","smd","bin","cue","z64","v64","nes", "sfc", "gba", "gbc", "wbfs","mdf"};
    std::vector<std::string> gGame;

    ROMBrowser::ROMBrowser(std::string path, SCREEN_UI::TextView* gamesPathView, SCREEN_UI::LayoutParams *layoutParams)
        : LinearLayout(SCREEN_UI::ORIENT_VERTICAL, layoutParams), m_Path(path), m_GamesPathView(gamesPathView)
    {
        using namespace SCREEN_UI;
        Refresh();
    }

    ROMBrowser::~ROMBrowser()
    {}

    void ROMBrowser::SetPath(const std::string &path)
    {
        m_Path.SetPath(path);
        Refresh();
    }

    std::string ROMBrowser::GetPath()
    {
        std::string str = m_Path.GetPath();
        return str;
    }

    void ROMBrowser::Update()
    {
        LinearLayout::Update();
        if (m_ListingPending && m_Path.IsListingReady())
        {
            Refresh();
        }
    }

    void ROMBrowser::Touch(const SCREEN_TouchInput &input)
    {
        for (auto iter = views_.begin(); iter != views_.end(); ++iter)
        {
            if ((*iter)->GetVisibility() == SCREEN_UI::V_VISIBLE)
            {
                (*iter)->Touch(input);
            }
        }
    }

    void ROMBrowser::Draw(SCREEN_UIContext &dc)
    {
        using namespace SCREEN_UI;

        for (View *view : views_)
        {
            if (view->GetVisibility() == V_VISIBLE)
            {
                // Check if bounds are in current scissor rectangle.
                if (dc.GetScissorBounds().Intersects(dc.TransformBounds(view->GetBounds())))
                {
                    view->Draw(dc);
                }
            }
        }
    }

    void ROMBrowser::Refresh()
    {
        using namespace SCREEN_UI;

        // Reset content
        Clear();

        Add(new Spacer(1.0f));
        auto mm = GetI18NCategory("MainMenu");

        SCREEN_UI::LinearLayout *linearLayout = new SCREEN_UI::LinearLayout(SCREEN_UI::ORIENT_VERTICAL, new LinearLayoutParams(FILL_PARENT, WRAP_CONTENT));
        linearLayout->SetSpacing(4.0f);
        m_GameList = linearLayout;

        Add(m_GameList);

        // Show games in the current directory
        m_DirButtons.clear();
        m_UpButton = nullptr;
        std::vector<ROMButton *> gameButtons;

        // Show folders in the current directory
        m_ListingPending = !m_Path.IsListingReady();

        std::vector<std::string> filenames;
        if (!m_ListingPending)
        {
            m_LastGamePath = m_Path.GetPath();
            m_GamesPathView->SetText(m_Path.GetFriendlyPath().c_str());

            std::list<std::string> tmpList;
            std::list<std::string> toBeRemoved;
            std::string strList;
            std::list<std::string>::iterator iteratorTmpList;

            std::string pathToBeSearched = m_Path.GetPath();
            FindAllFiles(pathToBeSearched.c_str(),&tmpList,&toBeRemoved,false);
            stripList(&tmpList,&toBeRemoved); // strip cue file entries
            finalizeList(&tmpList);

            iteratorTmpList = tmpList.begin();
            for (int i=0;i<tmpList.size();i++)
            {
                strList = *iteratorTmpList;
                iteratorTmpList++;
                gameButtons.push_back(new ROMButton(strList, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f)));
            }

            std::vector<File::FileInfo> fileInfo;
            m_Path.GetListing(fileInfo);
            uint buttonTextMaxLength = 40;
            for (size_t i = 0; i < fileInfo.size(); i++)
            {
                if (fileInfo[i].isDirectory)
                {
                    m_DirButtons.push_back(new DirButtonMain(
                                            fileInfo[i].fullName.c_str(),
                                            fileInfo[i].name,
                                            buttonTextMaxLength,
                                            new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f)));
                }
            }
        }

        if (m_LastGamePath != "/")
        {
            m_UpButton = new DirButtonMain("..", 2, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, 50.0f));
            m_UpButton->OnClick.Handle(this, &ROMBrowser::NavigateClick);
            m_GameList->Add(m_UpButton);
        }
        else
        {
            m_UpButton = nullptr;
        }

        if (m_ListingPending)
        {
            m_GameList->Add(new SCREEN_UI::TextView(mm->T("Loading..."), ALIGN_CENTER, false, new SCREEN_UI::LinearLayoutParams(SCREEN_UI::FILL_PARENT, SCREEN_UI::FILL_PARENT)));
        }

        for (size_t i = 0; i < gameButtons.size(); i++)
        {
            m_GameList->Add(gameButtons[i])->OnClick.Handle(this, &ROMBrowser::ROMButtonClick);
        }

        for (size_t i = 0; i < m_DirButtons.size(); i++)
        {
            std::string str = "ROMBrowser (" + std::to_string(i) + ") " + m_DirButtons[i]->GetPath();
            m_DirButtons[i]->SetTag(str);
            m_GameList->Add(m_DirButtons[i])->OnClick.Handle(this, &ROMBrowser::NavigateClick);
        }
    }

    const std::string ROMBrowser::GetBaseName(const std::string &path)
    {
        static const std::string sepChars = "/";

        auto trailing = path.find_last_not_of(sepChars);
        if (trailing != path.npos)
        {
            size_t start = path.find_last_of(sepChars, trailing);
            if (start != path.npos)
            {
                return path.substr(start + 1, trailing - start);
            }
            return path.substr(0, trailing);
        }

        size_t start = path.find_last_of(sepChars);
        if (start != path.npos)
        {
            return path.substr(start + 1);
        }
        return path;
    }

    SCREEN_UI::EventReturn ROMBrowser::NavigateClick(SCREEN_UI::EventParams &e)
    {
        DirButtonMain *button = static_cast<DirButtonMain *>(e.v);
        std::string text = button->GetPath();

        if (button->PathAbsolute())
        {
            m_Path.SetPath(text);
        }
        else
        {
            m_Path.Navigate(text);
        }
        Refresh();

        if (GetDefaultFocusView())
        {
            SCREEN_UI::SetFocusedView(GetDefaultFocusView());
        }
        else if (m_DirButtons.size())
        {
            SCREEN_UI::SetFocusedView(m_DirButtons[0]);
        }

        SCREEN_UI::EventParams onNavigateClickEvent{};
        onNavigateClickEvent.v = this;
        OnNavigateClick.Trigger(onNavigateClickEvent);

        return SCREEN_UI::EVENT_DONE;
    }

    SCREEN_UI::EventReturn ROMBrowser::ROMButtonClick(SCREEN_UI::EventParams &e)
    {
        ROMButton *button = static_cast<ROMButton *>(e.v);
        
        EmulatorLaunchEvent launchEvent(button->GetPath());
        m_EventCallback(launchEvent);

        return SCREEN_UI::EVENT_DONE;
    }

    void ROMBrowser::FindAllFiles(const char * directory, std::list<std::string> *tmpList, std::list<std::string> *toBeRemoved, bool recursiveSearch)
    {

        std::string str_with_path, str_without_path;
        std::string ext, str_with_path_lower_case;
        DIR *dir;

        struct dirent *ent;
        if ((dir = opendir (directory)) != nullptr)
        {
            // search all files and directories in directory
            while (((ent = readdir (dir)) != nullptr) && !stopSearching)
            {
                str_with_path = directory;
                str_with_path +=ent->d_name;

                if (isDirectory(str_with_path.c_str()) && (recursiveSearch))
                {
                    str_without_path =ent->d_name;
                    if ((str_without_path != ".") && (str_without_path != ".."))
                    {
                        str_with_path +="/";
                        FindAllFiles(str_with_path.c_str(),tmpList,toBeRemoved);
                    }
                }
                else
                {
                    ext = str_with_path.substr(str_with_path.find_last_of(".") + 1);

                    std::transform(ext.begin(), ext.end(), ext.begin(),
                        [](unsigned char c){ return std::tolower(c); });

                    str_with_path_lower_case=str_with_path;
                    std::transform(str_with_path_lower_case.begin(), str_with_path_lower_case.end(), str_with_path_lower_case.begin(),
                        [](unsigned char c){ return std::tolower(c); });

                    for (int i=0;i<gFileTypes.size();i++)
                    {
                        if ((ext == gFileTypes[i])  && \
                            (str_with_path_lower_case.find("battlenet") ==  std::string::npos) &&\
                            (str_with_path_lower_case.find("ps3") ==  std::string::npos) &&\
                            (str_with_path_lower_case.find("ps4") ==  std::string::npos) &&\
                            (str_with_path_lower_case.find("xbox") ==  std::string::npos) &&\
                            (str_with_path_lower_case.find("bios") ==  std::string::npos) &&\
                            (str_with_path_lower_case.find("firmware") ==  std::string::npos))
                        {
                            if (ext == "mdf")
                            {
                                std::string bin_file;
                                bin_file = str_with_path.substr(0,str_with_path.find_last_of(".")) + ".bin";
                                if (!exists(bin_file.c_str())) tmpList[0].push_back(str_with_path);
                            }
                            else if (ext == "cue")
                            {
                                if(checkForCueFiles(str_with_path,toBeRemoved))
                                  tmpList[0].push_back(str_with_path);
                            }
                            else
                            {
                                tmpList[0].push_back(str_with_path);
                            }
                        }
                    }
                }
            }
            closedir (dir);
        }
    }

    bool ROMBrowser::isDirectory(const char *filename)
    {
        struct stat p_lstatbuf;
        struct stat p_statbuf;
        bool ok = false;

        if (lstat(filename, &p_lstatbuf) < 0)
        {
            //printf("abort\n");
        }
        else
        {
            if (S_ISLNK(p_lstatbuf.st_mode) == 1)
            {
                //printf("%s is a symbolic link\n", filename);
            }
            else
            {
                if (stat(filename, &p_statbuf) < 0)
                {
                    //printf("abort\n");
                }
                else
                {
                    if (S_ISDIR(p_statbuf.st_mode) == 1)
                    {
                        //printf("%s is a directory\n", filename);
                        ok = true;
                    }
                }
            }
        }
        return ok;
    }

    void ROMBrowser::stripList(std::list<std::string> *tmpList,std::list<std::string> *toBeRemoved)
    {
        std::list<std::string>::iterator iteratorTmpList;
        std::list<std::string>::iterator iteratorToBeRemoved;

        std::string strRemove, strRemove_no_path, strList, strList_no_path;
        int i,j;

        iteratorToBeRemoved = toBeRemoved[0].begin();

        for (i=0;i<toBeRemoved[0].size();i++)
        {
            strRemove = *iteratorToBeRemoved;
            iteratorToBeRemoved++;
            iteratorTmpList = tmpList[0].begin();

            strRemove_no_path = strRemove;
            if(strRemove_no_path.find("/") != std::string::npos)
            {
                strRemove_no_path = strRemove.substr(strRemove_no_path.find_last_of("/") + 1);
            }

            for (j=0;j<tmpList[0].size();j++)
            {
                strList = *iteratorTmpList;

                strList_no_path = strList;
                if(strList_no_path.find("/") != std::string::npos)
                {
                    strList_no_path = strList_no_path.substr(strList_no_path.find_last_of("/") + 1);
                }

                if ( strRemove_no_path == strList_no_path )
                {
                    tmpList[0].erase(iteratorTmpList++);
                }
                else
                {
                    iteratorTmpList++;
                }
            }
        }
    }

    bool ROMBrowser::checkForCueFiles(std::string str_with_path, std::list<std::string> *toBeRemoved)
    {
        std::string line, name;
        bool file_exists = false;

        std::ifstream cueFile (str_with_path.c_str());
        if (!cueFile.is_open())
        {
            printf("Could not open cue file: %s\n",str_with_path.c_str());
        }
        else
        {
            while ( getline (cueFile,line))
            {
                if (line.find("FILE") != std::string::npos)
                {
                    str_with_path.substr(str_with_path.find_last_of(".") + 1);

                    int start  = line.find("\"")+1;
                    int length = line.find_last_of("\"")-start;
                    name = line.substr(start,length);

                    std::string name_with_path = name;
                    if (str_with_path.find("/") != std::string::npos)
                    {
                        name_with_path = str_with_path.substr(0,str_with_path.find_last_of("/")+1) + name;
                    }

                    if (exists(name.c_str()) || (exists(name_with_path.c_str())))
                    {
                        toBeRemoved[0].push_back(name);
                        file_exists = true;
                    } else return false;
                }
            }
        }
        return file_exists;
    }

    bool ROMBrowser::findInVector(std::vector<std::string>* vec, std::string str)
    {
        bool ok = false;
        std::string element;

        std::vector<std::string>::iterator it;

        for(it=vec[0].begin();it<vec[0].end();it++)
        {
            element = *it;
            if (element == str)
            {
                ok = true;
                break;
            }
        }
        return ok;
    }

    void ROMBrowser::finalizeList(std::list<std::string>* tmpList)
    {
        std::list<std::string>::iterator iteratorTmpList;
        std::string strList;

        iteratorTmpList = tmpList[0].begin();

        for (int i=0;i<tmpList[0].size();i++)
        {
            strList = *iteratorTmpList;
            if (!findInVector(&gGame,strList)) //avoid duplicates
            {
                gGame.push_back(strList);
            }
            iteratorTmpList++;
        }

        if (!gGame.size())
        {
            gGamesFound = false;
        }
        else
        {
            gGamesFound = true;
        }
    }

    bool ROMBrowser::exists(const char* filename)
    {
        std::ifstream infile(filename);
        return infile.good();
    }
    

    void ROMBrowser::SetEventCallback(const EventCallbackFunction& callback)
    {
        m_EventCallback = callback;
    }
}
