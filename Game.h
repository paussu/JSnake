//
// Created by jipe on 5/8/21.
//

#ifndef JSNAKE_GAME_H
#define JSNAKE_GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

class Game
{
public:
    Game(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* eventQueue);
    ~Game() = default;

    bool Initialize();

    void RunLoop();

    void Shutdown();
    void Pause();

private:

    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    void LoadData();
    void UnloadData();


    bool isRunning;
    bool isPaused;

    int mWidth, mHeight;

    ALLEGRO_DISPLAY* mDisplay;
    ALLEGRO_EVENT_QUEUE* mEventQueue;

};

#endif //JSNAKE_GAME_H
