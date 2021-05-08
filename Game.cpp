//
// Created by jipe on 5/8/21.
//

#include "Game.h"

Game::Game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* eventQueue)
: mDisplay(display), mEventQueue(eventQueue), isRunning(true), isPaused(false)
, mWidth(1280), mHeight(720)
{
    mSnake = std::make_unique<Snake>();
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

        if(event.type == ALLEGRO_EVENT_KEY_DOWN) ///look for keyboard events
        {
            if(event.keyboard.keycode == ALLEGRO_KEY_P)
            {
                isPaused = !isPaused;
            }

            if(isPaused) continue;

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

}

void Game::GenerateOutput()
{
    al_clear_to_color(ALLEGRO_COLOR{.r = 0, .g = 0, .b = 0, .a = 0});

    mSnake->Draw();

    al_flip_display();
}


