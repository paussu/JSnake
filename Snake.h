//
// Created by jipe on 5/8/21.
//

#pragma once

#include <list>
#include <allegro5/allegro_primitives.h>

class Snake
{
public:
    Snake();
    ~Snake() = default;

    void Grow();
    void Move(float x, float y);
    void Draw();
    bool CheckCollision();

    ALLEGRO_VERTEX &GetPosition();
    ALLEGRO_VERTEX &GetDirection();
private:
    std::list<ALLEGRO_VERTEX> mSnake;
    float mSnakeWidth;
    ALLEGRO_COLOR mSnakeColor;
    ALLEGRO_VERTEX mDirection;
};

