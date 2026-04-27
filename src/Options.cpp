#include "Options.h"
#include "Menu.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"

#include <algorithm>

Options::Options(const Menu *parentMenu)
{
    mParentMenu = parentMenu;
}

void Options::Draw()
{
    if (!mIsShown)
        return;

    const auto &parentWidth = mParentMenu->GetWidth();
    const auto &parentHeight = mParentMenu->GetHeight();
    const auto &configuration = GetGameConfiguration();

    const float windowWidth = std::min(parentWidth * 0.56f, 720.0f);
    const float windowHeight = std::min(parentHeight * 0.62f, 420.0f);

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2((parentWidth - windowWidth) * 0.5f, (parentHeight - windowHeight) * 0.5f), ImGuiCond_Always);
    ImGui::Begin("Options", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    ImGui::TextColored(ImVec4(0.27f, 0.80f, 0.62f, 1.0f), "Game setup");
    ImGui::TextDisabled("Tune the display and choose the look of the snake.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("OptionsEditor", ImVec2(windowWidth * 0.52f, 0.0f), true);
    ImGui::TextDisabled("Video");
    ImGui::SetNextItemWidth(-1.0f);
    ImGui::Combo("Resolution", &mSelectedResolution, comboItems, IM_ARRAYSIZE(comboItems));
    ImGui::Checkbox("Fullscreen", &mGameConfiguration.fullscreen);
    ImGui::Spacing();
    ImGui::TextDisabled("Visuals");
    ImGui::Checkbox("Use sprite graphics", &mGameConfiguration.useSprites);
    ImGui::Spacing();
    ImGui::TextWrapped("Changes apply to the next run when you start a new game.");
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("OptionsPreview", ImVec2(0.0f, 0.0f), true);
    ImGui::TextColored(ImVec4(0.98f, 0.91f, 0.39f, 1.0f), "Preview");
    ImGui::Spacing();
    ImGui::BulletText("Resolution: %dx%d", configuration.screenWidth, configuration.screenHeight);
    ImGui::BulletText("Display: %s", configuration.fullscreen ? "Fullscreen" : "Windowed");
    ImGui::BulletText("Theme: %s", configuration.useSprites ? "Sprite pack" : "Minimal mode");
    ImGui::Spacing();
    ImGui::TextWrapped("Higher resolutions create a larger playfield. Sprite mode uses the art in the Assets folder, while minimal mode keeps the classic block style.");
    ImGui::Dummy(ImVec2(0.0f, 16.0f));

    if (ImGui::Button("Back to menu", ImVec2(-1.0f, 46.0f)))
        mIsShown = false;

    ImGui::EndChild();

    ImGui::End();
}

void Options::SetShown()
{
    mIsShown = true;
}

const GameConfiguration& Options::GetGameConfiguration()
{
    switch (mSelectedResolution)
    {
        case GameResolution::Resolution_800x600:
            mGameConfiguration.screenWidth = 800;
            mGameConfiguration.screenHeight = 600;
            break;
        case GameResolution::Resolution_1024x768:
            mGameConfiguration.screenWidth = 1024;
            mGameConfiguration.screenHeight = 768;
            break;
        case GameResolution::Resolution_1280x720:
            mGameConfiguration.screenWidth = 1280;
            mGameConfiguration.screenHeight = 720;
            break;
        case GameResolution::Resolution_1440x900:
            mGameConfiguration.screenWidth = 1440;
            mGameConfiguration.screenHeight = 900;
            break;
        case GameResolution::Resolution_1600x900:
            mGameConfiguration.screenWidth = 1600;
            mGameConfiguration.screenHeight = 900;
            break;
        case GameResolution::Resolution_1920x1080:
            mGameConfiguration.screenWidth = 1920;
            mGameConfiguration.screenHeight = 1080;
            break;
        case GameResolution::Resolution_2560x1440:
            mGameConfiguration.screenWidth = 2560;
            mGameConfiguration.screenHeight = 1440;
            break;
        default:
            mGameConfiguration.screenWidth = 1024;
            mGameConfiguration.screenHeight = 768;
            break;
    }
    return mGameConfiguration;
}


