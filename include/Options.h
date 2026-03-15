//
// Created by jipe on 5/26/21.
//

#pragma once

struct GameConfiguration
{
    int screenWidth, screenHeight;
    bool fullscreen;
    bool useSprites;
};

enum GameResolution
{
    Resolution_800x600 = 0,
    Resolution_1024x768,
    Resolution_1280x720,
    Resolution_1440x900,
    Resolution_1600x900,
    Resolution_1920x1080,
    Resolution_2560x1440
};

class Options
{
 public:
    explicit Options(const class Menu* parentMenu);
    ~Options() = default;

    void Draw();
    void SetShown();

    [[nodiscard]] const GameConfiguration & GetGameConfiguration();

 private:
    const class Menu* mParentMenu = nullptr;
    bool isShowed = false;
    int mSelectedResolution = 1;
    const char *comboItems[7] = {"800x600", "1024x768", "1280x720", "1440x900", "1600x900", "1920x1080", "2560x1440"};

    GameConfiguration mGameConfiguration{1024, 768, false, true};
};


