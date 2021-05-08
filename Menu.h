//
// Created by jipe on 5/8/21.
//

#ifndef JSNAKE_MENU_H
#define JSNAKE_MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "imgui.h"
#include "imgui_impl_allegro5.h"


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
    void GenerateOutput();

    bool isRunning;
    bool gameStartRequested;
    bool showMenu;
    ImVec4 clearColor;
    int mWidth, mHeight;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;

    void RunGame();
};

#endif //JSNAKE_MENU_H
