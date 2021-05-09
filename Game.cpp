//
// Created by jipe on 5/8/21.
//

#include "Game.h"
#include <time.h>

Game::Game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* eventQueue)
: mDisplay(display), mEventQueue(eventQueue), isRunning(true), isPaused(false)
, mWidth(1280), mHeight(720), mRandomEngine(std::chrono::steady_clock::now().time_since_epoch().count())
, previousTime(0), currentTime(0), mSpeed(2.0)
{
    mSnake = std::make_unique<Snake>();
    mFood = std::make_unique<Food>();
}

bool Game::Initialize()
{
    // Setup Allegro
    al_init();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    mDisplay = al_create_display(mWidth, mHeight);
    al_set_window_title(mDisplay, "JSnake");
    mEventQueue = al_create_event_queue();
    al_register_event_source(mEventQueue, al_get_display_event_source(mDisplay));
    al_register_event_source(mEventQueue, al_get_keyboard_event_source());
    al_register_event_source(mEventQueue, al_get_mouse_event_source());

    mTimer = al_create_timer(0.1);
    al_start_timer(mTimer);

    return true;
}

void Game::RunLoop()
{
    while (isRunning)
    {
        ProcessInput();
        if(!isPaused)
        {
            UpdateGame();
            GenerateOutput();
        }
    }
}

void Game::Shutdown()
{
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

        if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_P)
            {
                isPaused = !isPaused;
            }

        }

        if(isPaused) return;

        if(event.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_UP)
            {
                mSnake->Move(0, -10);
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
            {
                mSnake->Move(0, 10);
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
            {
                mSnake->Move(-10, 0);
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
            {
                mSnake->Move(10, 0);
            }
        }

    }
}

void Game::UpdateGame()
{
    if(mFood->IsEaten(mSnake->GetPosition()))
    {
        mFood->SetPosition(RandomPosition(10));
        mSnake->Grow();

        const auto &timerSpeed = al_get_timer_speed(mTimer);
        if(timerSpeed > 0.001)
            al_set_timer_speed(mTimer, timerSpeed - 0.001);
    }

    if(mSnake->GetPosition().x > mWidth) mSnake->GetPosition().x = 0;
    if(mSnake->GetPosition().x < 0) mSnake->GetPosition().x = mWidth;
    if(mSnake->GetPosition().y > mHeight) mSnake->GetPosition().y = 0;
    if(mSnake->GetPosition().y < 0) mSnake->GetPosition().y = mHeight;

    currentTime = al_get_timer_count(mTimer);
    if(previousTime + mSpeed < currentTime)
    {
        mSnake->Move(10 * mSnake->GetPreviousDirection().x, 10 * mSnake->GetPreviousDirection().y);
        previousTime = currentTime;
    }

    if(mSnake->CheckCollision())
        isRunning = false;
}

void Game::GenerateOutput()
{
    al_clear_to_color(ALLEGRO_COLOR{.r = 0, .g = 0, .b = 0, .a = 0});

    mSnake->Draw();
    mFood->Draw();

    al_flip_display();
}

ALLEGRO_VERTEX Game::RandomPosition(float foodSize)
{
    ALLEGRO_VERTEX randomVertex;

    std::uniform_int_distribution<int> xDistribution {10, mWidth};
    randomVertex.x = xDistribution(mRandomEngine);
    randomVertex.x -= fmodf(randomVertex.x, foodSize);
    std::uniform_int_distribution<int> yDistribution {10, mHeight};
    randomVertex.y = yDistribution(mRandomEngine);
    randomVertex.y -= fmodf(randomVertex.y, foodSize);

    return randomVertex;
}


