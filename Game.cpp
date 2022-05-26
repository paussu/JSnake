//
// Created by jipe on 5/8/21.
//
#include "Game.h"
#include "Options.h"
#include "Snake.h"
#include "Food.h"

#include <allegro5/allegro_image.h>
#include <chrono>

Game::Game(const GameConfiguration* config)
: mConfiguration(config), mDisplay(nullptr), mEventQueue(nullptr), mTimer(nullptr), mFont(nullptr), isRunning(true), isPaused(false)
, gameLost(false), mHudHeight(16), mRandomEngine(std::chrono::steady_clock::now().time_since_epoch().count())
, previousTime(0), currentTime(0), mSpeed(10.5), mFontSize(16), mScore(0), mScoreText("Score: 0")
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
    // Setup Allegro
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_init_image_addon();

    if (mConfiguration->fullscreen)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    else
        al_set_new_display_flags(ALLEGRO_RESIZABLE);

    mDisplay = al_create_display(mConfiguration->screenWidth, mConfiguration->screenHeight);
    al_set_window_position(mDisplay, mConfiguration->screenWidth / 2, mConfiguration->screenHeight / 2);
    al_set_window_title(mDisplay, "JSnake");

    mEventQueue = al_create_event_queue();
    al_register_event_source(mEventQueue, al_get_display_event_source(mDisplay));
    al_register_event_source(mEventQueue, al_get_keyboard_event_source());
    al_register_event_source(mEventQueue, al_get_mouse_event_source());

    mFont = al_load_ttf_font("../Assets/RETRO_SPACE_INV.ttf", mFontSize, 0);
    if (mFont == nullptr)
        return false;

    mTimer = al_create_timer(0.1);
    al_start_timer(mTimer);

    mSnake = std::make_unique<Snake>(mConfiguration->useSprites);
    mFood = std::make_unique<Food>(mConfiguration->useSprites);

    return true;
}

void Game::RunLoop()
{
    while (isRunning)
    {
        ProcessInput();

        if (!isPaused && !gameLost)
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

        if (gameLost)
        {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN
            && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    isRunning = false;

            continue;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_P)
                isPaused = !isPaused;

            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                isRunning = false;
        }

        if (isPaused)
            return;

        if (event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                mSnake->Move(0, -mSnake->GetSize());
                break;
            case ALLEGRO_KEY_DOWN:
                mSnake->Move(0, mSnake->GetSize());
                break;
            case ALLEGRO_KEY_LEFT:
                mSnake->Move(-mSnake->GetSize(), 0);
                break;
            case ALLEGRO_KEY_RIGHT:
                mSnake->Move(mSnake->GetSize(), 0);
                break;
            default:
                break;
            }
        }
    }
}

void Game::UpdateGame()
{
    if (mFood->IsEaten(mSnake->GetPosition()))
    {
        mFood->SetPosition(RandomPosition(mSnake->GetSize()));
        mSnake->Grow();

        const auto &timerSpeed = al_get_timer_speed(mTimer);
        if (timerSpeed > 0.001)
            al_set_timer_speed(mTimer, timerSpeed - 0.001);

        mScore++;
        mScoreText = "Score: " + std::to_string(mScore);
    }

    if (mSnake->GetPosition().x > mConfiguration->screenWidth)
        mSnake->GetPosition().x = 0;

    if (mSnake->GetPosition().x < 0)
        mSnake->GetPosition().x = mConfiguration->screenWidth - mConfiguration->screenWidth % static_cast<int>(mSnake->GetSize());

    if (mSnake->GetPosition().y > mConfiguration->screenHeight)
        mSnake->GetPosition().y = mHudHeight;

    if (mSnake->GetPosition().y < mHudHeight)
        mSnake->GetPosition().y = mConfiguration->screenHeight - mConfiguration->screenHeight % static_cast<int>(mSnake->GetSize());

    currentTime = al_get_timer_count(mTimer);
    if (previousTime + mSpeed < currentTime)
    {
        mSnake->Move(mSnake->GetSize() * mSnake->GetDirection().x, mSnake->GetSize() * mSnake->GetDirection().y);
        previousTime = currentTime;
    }

    if (mSnake->CheckCollision())
        gameLost = true;
}

void Game::GenerateOutput()
{
    al_clear_to_color(ALLEGRO_COLOR{.r = 0, .g = 0, .b = 0, .a = 0});

    if (isPaused)
        DrawPauseMessage();
    else
        DrawGame();

#ifdef DEBUG_MODE
    for(int x = 0; x < mConfiguration->screenWidth; x += mSnake->GetSize())
    {
        al_draw_line(x, mHudHeight, x, mConfiguration->screenHeight, al_map_rgb(255, 0, 0), 1);
    }
    for(int y = 0; y < mConfiguration->screenHeight; y += mSnake->GetSize())
    {
        al_draw_line(0, y, mConfiguration->screenWidth, y, al_map_rgb(255, 0, 0), 1);
    }
#endif

    al_flip_display();
}

ALLEGRO_VERTEX Game::RandomPosition(float foodSize)
{
    ALLEGRO_VERTEX randomVertex;

    std::uniform_int_distribution<int> xDistribution {0, mConfiguration->screenWidth};
    randomVertex.x = xDistribution(mRandomEngine);
    randomVertex.x -= fmodf(randomVertex.x, foodSize);

    std::uniform_int_distribution<int> yDistribution {mHudHeight, mConfiguration->screenHeight};
    randomVertex.y = yDistribution(mRandomEngine);
    randomVertex.y -= fmodf(randomVertex.y, foodSize);

    if (mSnake->IsInside(randomVertex))
        return(RandomPosition(foodSize));

    return randomVertex;
}

void Game::DrawGame()
{
    al_draw_line(0, mHudHeight, mConfiguration->screenWidth, mHudHeight, al_map_rgb(255, 255, 255), 2);
    al_draw_text(mFont, al_map_rgb(255, 255, 255), 0, 0, 0, mScoreText.c_str());
    mSnake->Draw();
    mFood->Draw();

    if (gameLost)
    {
        al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration->screenWidth / 2.0, mConfiguration->screenHeight / 4.0,
                     ALLEGRO_ALIGN_CENTER, "YOU LOST");
        al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration->screenWidth / 2.0, mConfiguration->screenHeight / 2.0, ALLEGRO_ALIGN_CENTER, "Press ESC to return to menu");
    }
}

void Game::DrawPauseMessage()
{
    al_draw_text(mFont, al_map_rgb(255, 255, 255), mConfiguration->screenWidth / 2.0, mConfiguration->screenHeight / 4.0, ALLEGRO_ALIGN_CENTER, "GAME PAUSED");
}

