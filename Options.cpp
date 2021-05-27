//
// Created by jipe on 5/26/21.
//

#include "Options.h"
#include "Menu.h"

Options::Options(const Menu *parentMenu)
: mParentMenu(parentMenu), isShowed(false), mGameConfiguration{1024, 768, false}
, comboItems{"800x600", "1024x768", "1280x720", "1440x900", "1600x900", "1920x1080", "2560x1440"}
, mSelectedResolution(1)
{

}

void Options::Draw()
{
    if(!isShowed)
        return;

    const auto &parentWidth = mParentMenu->GetWidth();
    const auto &parentHeight = mParentMenu->GetHeight();

    ImGui::SetNextWindowSize(ImVec2(parentWidth, parentHeight - (parentHeight / 4)), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(parentWidth / 4, 100));
    ImGui::Begin("Options", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Combo("Resolution", &mSelectedResolution, comboItems, IM_ARRAYSIZE(comboItems));
    ImGui::Checkbox("Fullscreen", &mGameConfiguration.Fullscreen);
    if (ImGui::Button("Exit Options", ImVec2(parentWidth  / 4, 50)))
        isShowed = false;

    ImGui::End();
}

void Options::SetShown()
{
    isShowed = true;
}

const GameConfiguration & Options::GetGameConfiguration()
{
    switch (mSelectedResolution)
    {
        case GameResolution::Resolution_800x600:
            mGameConfiguration.ScreenWidth = 800;
            mGameConfiguration.ScreenHeight = 600;
            break;
        case GameResolution::Resolution_1024x768:
            mGameConfiguration.ScreenWidth = 1024;
            mGameConfiguration.ScreenHeight = 768;
            break;
        case GameResolution::Resolution_1280x720:
            mGameConfiguration.ScreenWidth = 1280;
            mGameConfiguration.ScreenHeight = 720;
            break;
        case GameResolution::Resolution_1440x900:
            mGameConfiguration.ScreenWidth = 1440;
            mGameConfiguration.ScreenHeight = 900;
            break;
        case GameResolution::Resolution_1600x900:
            mGameConfiguration.ScreenWidth = 1600;
            mGameConfiguration.ScreenHeight = 900;
            break;
        case GameResolution::Resolution_1920x1080:
            mGameConfiguration.ScreenWidth = 1920;
            mGameConfiguration.ScreenHeight = 1080;
            break;
        case GameResolution::Resolution_2560x1440:
            mGameConfiguration.ScreenWidth = 2560;
            mGameConfiguration.ScreenHeight = 1440;
            break;
        default:
            mGameConfiguration.ScreenWidth = 1024;
            mGameConfiguration.ScreenHeight = 768;
            break;
    }
    return mGameConfiguration;
}


