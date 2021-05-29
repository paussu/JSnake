//
// Created by jipe on 5/8/21.
//
#include "Game.h"
#include "Options.h"

#include <chrono>

Game::Game(GameConfiguration config)
: mConfiguration(config), mDisplay(nullptr), mEventQueue(nullptr), mTimer(nullptr), mFont(nullptr), isRunning(true), isPaused(false)
, gameLost(false), mSnakeSize(10), mHudHeight(20), mRandomEngine(std::chrono::steady_clock::now().time_since_epoch().count())
, previousTime(0), currentTime(0), mSpeed(2.0), mFontSize(20), mScore(0), mScoreText("Score: 0")
{
    mSnake = std::make_unique<Snake>();
    mFood = std::make_unique<Food>();
}

bool Game::Initialize()
{
    // Setup Allegro
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_ttf_addon();

    if(mConfiguration.Fullscreen)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    else
        al_set_new_display_flags(ALLEGRO_RESIZABLE);

    mDisplay = al_create_display(mConfiguration.ScreenWidth, mConfiguration.ScreenHeight);
    al_set_window_position(mDisplay, mConfiguration.ScreenWidth / 2, mConfiguration.ScreenHeight / 2);
    al_set_window_title(mDisplay, "JSnake");

    mEventQueue = al_create_event_queue();
    al_register_event_source(mEventQueue, al_get_display_event_source(mDisplay));
    al_register_event_source(mEventQueue, al_get_keyboard_event_source());
    al_register_event_source(mEventQueue, al_get_mouse_event_source());

    mFont = al_load_ttf_font("../Assets/RETRO_SPACE_INV.ttf", mFontSize, 0);
    if(mFont == nullptr)
        return false;

    mTimer = al_create_timer(0.1);
    al_start_timer(mTimer);

    return true;
}

void Game::RunLoop()
{
    while (isRunning)
    {
        ProcessInput();

        if(!isPaused && !gameLost)
            UpdateGame();

        GenerateOutput();
    }
}

void Game::Shutdown()
{
    al_destroy_font(mFont);
    al_destroy_event_queue(mEventQueue);
    al_destroy_display(mDisplay);
}

void Game::ProcessInput()
{
    // Poll and handle events (inputs, window resize, etc.)
    ALLEGRO_EVENT event;
    while (al_get_next_event(mEventQueue, &event))
    {
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            isRunning = false;

        if(gameLost)
        {
            if(event.type == ALLEGRO_EVENT_KEY_DOWN
            && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    isRunning = false;

            continue;
        }

        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_P)
                isPaused = !isPaused;

            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                isRunning = false;
        }

        if(isPaused) return;

        if(event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                mSnake->Move(0, -mSnakeSize);

            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                mSnake->Move(0, mSnakeSize);

            if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                mSnake->Move(-mSnakeSize, 0);

            if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                mSnake->Move(mSnakeSize, 0);
        }

    }
}

void Game::UpdateGame()
{
    if(mFood->IsEaten(mSnake->GetPosition()))
    {
        mFood->SetPosition(RandomPosition(mSnakeSize));
        mSnake->Grow();

        const auto &timerSpeed = al_get_timer_speed(mTimer);
        if(timerSpeed > 0.001)
            al_set_timer_speed(mTimer, timerSpeed - 0.001);

        mScore++;
        mScoreText = "Score: " + std::to_string(mScore);
    }

    if(mSnake->GetPosition().x > mConfiguration.ScreenWidth) mSnake->GetPosition().x = 0;
    if(mSnake->GetPosition().x < 0) mSnake->GetPosition().x = mConfiguration.ScreenWidth - mConfiguration.ScreenWidth % mSnakeSize;
    if(mSnake->GetPosition().y > mConfiguration.ScreenHeight) mSnake->GetPosition().y = mHudHeight;
    if(mSnake->GetPosition().y < mHudHeight) mSnake->GetPosition().y = mConfiguration.ScreenHeight - mConfiguration.ScreenHeight % mSnakeSize;

    currentTime = al_get_timer_count(mTimer);
    if(previousTime + mSpeed < currentTime)
    {
        mSnake->Move(mSnakeSize * mSnake->GetDirection().x, mSnakeSize * mSnake->GetDirection().y);
        previousTime = currentTime;
    }

    if(mSnake->CheckCollision())
        gameLost = true;
}

void Game::GenerateOutput()
{
    al_clear_to_color(ALLEGRO_COLOR{.r = 0, .g = 0, .b = 0, .a = 0});

    if(isPaused)
        DrawPauseMessage();
    else
        DrawGame();

#ifdef DEBUG_MODE
    for(int x = 0; x < mConfiguration.ScreenWidth; x += mSnakeSize)
    {
        al_draw_line(x, mHudHeight, x, mConfiguration.ScreenHeight, al_map_rgb(255, 0, 0), 1);
    }
    for(int y = mHudHeight; y < mConfiguration.ScreenHeight; y += mSnakeSize)
    {
        al_draw_line(0, y, mConfiguration.ScreenWidth, y, al_map_rgb(255, 0, 0), 1);
    }
#endif

    al_flip_display();
}

ALLEGRO_VERTEX Game::RandomPosition(float foodSize)
{
    ALLEGRO_VERTEX randomVertex;

    std::uniform_int_distribution<int> xDistribution {0, mConfiguration.ScreenWidth};
    randomVertex.x = xDistribution(mRandomEngine);
    randomVertex.x -= fmodf(randomVertex.x, foodSize);

    std::uniform_int_distribution<int> yDistribution {mHudHeight, mConfiguration.ScreenHeight};
    randomVertex.y = yDistribution(mRandomEngine);
    randomVertex.y -= fmodf(randomVertex.y, foodSize);

    if(mSnake->IsInside(randomVertex))
        return(RandomPosition(foodSize));

    return randomVertex;
}

void Game::DrawGame()
{
    al_draw_line(0, mHudHeight, mConfiguration.ScreenWidth, mHudHeight, al_map_rgb(255, 255, 255), 2);
    al_draw_text(mFont, al_map_rgb(255, 255, 255), 0, 0, 0, mScoreText.c_str());
    mSnake->Draw();
    mFood->Draw();

    if(gameLost)
    {
        al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration.ScreenWidth / 2, mConfiguration.ScreenHeight / 4,
                     ALLEGRO_ALIGN_CENTER, "YOU LOST");
        al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration.ScreenWidth / 2, mConfiguration.ScreenHeight / 2, ALLEGRO_ALIGN_CENTER, "Press ESC to return to menu");
    }
}

void Game::DrawPauseMessage()
{
    al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration.ScreenWidth / 2, mConfiguration.ScreenHeight / 4, ALLEGRO_ALIGN_CENTER, "GAME PAUSED");
}

