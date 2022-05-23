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
    const class Menu* mParentMenu;
    bool isShowed;
    int mSelectedResolution;
    const char *comboItems[7];

    GameConfiguration mGameConfiguration;
};


