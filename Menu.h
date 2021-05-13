//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_allegro5.h"

class Menu
{
public:
    Menu();
    ~Menu() = default;

    bool Initialize();

    void RunLoop();

    void Shutdown();
private:
    void ProcessInput();
    void DrawMenu();
    void DrawOptions();
    void GenerateOutput();

    bool isRunning;
    bool gameStartRequested;
    bool showMenu;
    bool showOptions;

    ImVec4 clearColor;
    int mWidth, mHeight;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;

    void RunGame();
};

