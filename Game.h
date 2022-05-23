//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <memory>
#include <random>

struct GameConfiguration;
class Food;
class Snake;

class Game
{
public:
    explicit Game(const GameConfiguration* config);
    ~Game();

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
    bool gameLost;

    const GameConfiguration* mConfiguration;
    int mHudHeight;
    int mFontSize;
    int mScore;
    int previousTime;
    int currentTime;
    double mSpeed;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;
    ALLEGRO_FONT* mFont;
    ALLEGRO_TIMER* mTimer;
    std::unique_ptr<Snake> mSnake;
    std::unique_ptr<Food> mFood;

    std::default_random_engine mRandomEngine;
    std::string mScoreText;
};
