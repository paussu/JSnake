#include "Hiscores.h"
#include "Logger.h"
#include "Menu.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"
#include <algorithm>
#include <fstream>

Hiscores::Hiscores(const Menu *parentMenu)
{
    mParentMenu = parentMenu;
    LoadFromFile();
}

void Hiscores::Draw()
{
    if (!mIsShown)
        return;

    const auto &parentWidth = mParentMenu->GetWidth();
    const auto &parentHeight = mParentMenu->GetHeight();
    const float windowWidth = std::min(parentWidth * 0.5f, 640.0f);
    const float windowHeight = std::min(parentHeight * 0.7f, 540.0f);

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2((parentWidth - windowWidth) * 0.5f, (parentHeight - windowHeight) * 0.5f), ImGuiCond_Always);
    ImGui::Begin("Hiscores", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    ImGui::TextColored(ImVec4(0.27f, 0.80f, 0.62f, 1.0f), "Hall of fame");
    ImGui::TextDisabled("The best surviving snakes are listed here.");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginChild("HiscoreList", ImVec2(0.0f, -58.0f), true);
    if (mScoreList.empty())
    {
        ImGui::Dummy(ImVec2(0.0f, 40.0f));
        ImGui::TextDisabled("No scores saved yet. Finish a game to create the first record.");
    }
    else
    {
        ImGui::Columns(3, "HiscoreColumns", false);
        ImGui::TextColored(ImVec4(0.98f, 0.91f, 0.39f, 1.0f), "#");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.98f, 0.91f, 0.39f, 1.0f), "Player");
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.98f, 0.91f, 0.39f, 1.0f), "Score");
        ImGui::NextColumn();
        ImGui::Separator();

        int rank = 1;
        for (const auto& score : mScoreList)
        {
            ImGui::Text("%d", rank++);
            ImGui::NextColumn();
            ImGui::TextUnformatted(score.first.c_str());
            ImGui::NextColumn();
            ImGui::Text("%d", score.second);
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
    }
    ImGui::EndChild();

    if (ImGui::Button("Back to menu", ImVec2(-1.0f, 42.0f)))
        mIsShown = false;

    ImGui::End();
}

void Hiscores::SetShown()
{
    mIsShown = true;
}

void Hiscores::LoadFromFile()
{
    mScoreList.clear();

    std::ifstream scoreFile;
    std::string line;

    scoreFile.open("../Hiscores.score");
    if (!scoreFile.is_open())
    {
        Logger::Debug("No hiscore file found yet at ../Hiscores.score");
        return;
    }

    Logger::Debug("Loading hiscores from ../Hiscores.score");

    while (getline(scoreFile, line, '\n'))
    {
        const auto delimiterPos = line.find(';');
        const std::string playerName = line.substr(0, delimiterPos);
        line.erase(0, delimiterPos + 1);
        const int score = std::stoi(line);
        mScoreList.push_back({playerName, score});
    }

    std::sort(mScoreList.begin(), mScoreList.end(), [](const std::pair<std::string, int> &score1, const std::pair<std::string, int> &score2)
    {
        return score1.second > score2.second;
    });

    Logger::Debug("Loaded " + std::to_string(mScoreList.size()) + " hiscore entries");
}

void Hiscores::SaveScore(const std::string &playerName, int score)
{
    std::ofstream scoreFile;
    std::string line;

    scoreFile.open("../Hiscores.score", std::ios_base::app);
    if (!scoreFile.is_open())
    {
        Logger::Error("Failed to open hiscore file for writing");
        return;
    }

    scoreFile << playerName << ";" << score << std::endl;   
    Logger::Debug("Saved score entry for " + playerName);
}
