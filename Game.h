//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <memory>
#include <random>
#include <chrono>
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

    ALLEGRO_VERTEX RandomPosition(float foodSize);

    bool isRunning;
    bool isPaused;

    int mWidth, mHeight;
    double mSpeed;
    int previousTime;
    int currentTime;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;
    ALLEGRO_TIMER* mTimer;
    std::unique_ptr<Snake> mSnake;
    std::unique_ptr<Food> mFood;

    std::default_random_engine mRandomEngine;
};
