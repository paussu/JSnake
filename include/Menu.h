//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"

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
    void DrawOptions();
    void GenerateOutput();
    void RunGame();

   bool isRunning = true;
   bool gameStartRequested = false;
   bool showMenu = true;
   bool showOptions = false;

   ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
   int mWidth = 1280;
   int mHeight = 720;

   ALLEGRO_DISPLAY* mDisplay = nullptr;
   ALLEGRO_EVENT_QUEUE* mEventQueue = nullptr;

    std::unique_ptr<Options> mOptions;
    std::unique_ptr<Hiscores> mHiscores;
};

