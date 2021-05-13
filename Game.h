//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <memory>
#include <random>

#include "Snake.h"
#include "Food.h"

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
    void DrawGame();
    void DrawPauseMessage();

    ALLEGRO_VERTEX RandomPosition(float foodSize);

    bool isRunning;
    bool isPaused;

    int mWidth, mHeight;
    int mFontSize;
    int previousTime;
    int currentTime;
    double mSpeed;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;
    ALLEGRO_FONT* mFont;
    ALLEGRO_TIMER* mTimer;
    std::unique_ptr<class Snake> mSnake;
    std::unique_ptr<class Food> mFood;

    std::default_random_engine mRandomEngine;
};
