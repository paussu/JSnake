//
// Created by jipe on 5/8/21.
//

#include "Game.h"

Game::Game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* eventQueue)
: mDisplay(display), mEventQueue(eventQueue), isRunning(true), isPaused(false)
, mWidth(1280), mHeight(720)
{

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
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    al_destroy_event_queue(mEventQueue);
    al_destroy_display(mDisplay);
}

void Game::Pause()
{

}

void Game::ProcessInput()
{
// Poll and handle events (inputs, window resize, etc.)
    ALLEGRO_EVENT ev;
    while (al_get_next_event(mEventQueue, &ev))
    {
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            isRunning = false;
    }
}

void Game::UpdateGame()
{

}

void Game::GenerateOutput()
{
    al_draw_filled_triangle(35, 350, 85, 375, 35, 400, al_map_rgb_f(0, 1, 0));
    al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
    al_draw_circle(450, 370, 30, al_map_rgb_f(1, 0, 1), 2);
    al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
    al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);
    al_flip_display();
}

void Game::LoadData()
{

}

void Game::UnloadData()
{

}

