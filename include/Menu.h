#pragma once

#include <allegro5/allegro.h>
#include <memory>

class Hiscores;
class Options;

class Menu
{
 public:
    Menu();
    ~Menu();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    [[nodiscard]] int GetWidth() const;
    [[nodiscard]] int GetHeight() const;

 private:
    void ProcessInput();
    void DrawMenu();
    void DrawMenuBackground(int width, int height);
    void GenerateOutput();
    void RunGame();

    bool mIsRunning = true;
    bool mGameStartRequested = false;

    int mWidth = 1280;
    int mHeight = 720;

    ALLEGRO_DISPLAY* mDisplay = nullptr;
    ALLEGRO_EVENT_QUEUE* mEventQueue = nullptr;

    std::unique_ptr<Options> mOptions;
    std::unique_ptr<Hiscores> mHiscores;
};

