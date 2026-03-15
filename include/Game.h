//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <chrono>
#include <memory>
#include <random>
#include <string>

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
    int GetScore();
    std::string GetPlayerName();

 private:
    void ProcessInput();
    void HandleDisplayCloseEvent(const ALLEGRO_EVENT& event);
    void HandleGameLostEvent(const ALLEGRO_EVENT& event);
    void HandleKeyDownEvent(const ALLEGRO_EVENT& event);
    void HandleKeyCharEvent(const ALLEGRO_EVENT& event);
    void HandleMovementKey(int keycode);
    void UpdateGame();
    void HandleSnakeBoundary();
    void GenerateOutput();
    void DrawGame();
    void DrawPauseMessage();
    void GameOverScreen();

    ALLEGRO_VERTEX RandomPosition(float foodSize);

   bool isRunning = true;
   bool isPaused = false;
   bool gameLost = false;

   const GameConfiguration* mConfiguration = nullptr;
   int mHudHeight = 16;
   int mFontSize = 16;
   int mScore = 0;
   int previousTime = 0;
   int currentTime = 0;
   double mSpeed = 10.5;

   ALLEGRO_DISPLAY* mDisplay = nullptr;
   ALLEGRO_EVENT_QUEUE* mEventQueue = nullptr;
   ALLEGRO_FONT* mFont = nullptr;
   ALLEGRO_TIMER* mTimer = nullptr;
    std::unique_ptr<Snake> mSnake;
    std::unique_ptr<Food> mFood;

   std::default_random_engine mRandomEngine{
      static_cast<std::default_random_engine::result_type>(std::chrono::steady_clock::now().time_since_epoch().count())
   };
   std::string mScoreText = "Score: 0";
    std::string mPlayerName;
};
