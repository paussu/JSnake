//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"
#include "Options.h"

class Menu
{
public:
    Menu();
    ~Menu() = default;

    bool Initialize();

    void RunLoop();

    void Shutdown();

    [[nodiscard]] int GetWidth() const;

    [[nodiscard]] int GetHeight() const;

private:
    void ProcessInput();
    void DrawMenu();
    void DrawOptions();
    void GenerateOutput();
    void RunGame();

    bool isRunning;
    bool gameStartRequested;
    bool showMenu;
    bool showOptions;

    ImVec4 clearColor;
    int mWidth, mHeight;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;

    std::unique_ptr<Options> mOptions;
};

