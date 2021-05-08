//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include "Snake.h"

class Game
{
public:
    Game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* eventQueue);
    ~Game() = default;

    bool Initialize();
    void RunLoop();
    void Shutdown();

private:

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    bool isRunning;
    bool isPaused;

    int mWidth, mHeight;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;
    std::unique_ptr<Snake> mSnake;
};
