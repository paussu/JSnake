//
// Created by jipe on 5/8/21.
//

#include <ranges>
#include "Snake.h"

Snake::Snake()
: mSnakeWidth(10.0f), mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255}, mDirection{.x = -1, .y = 0}
{
    mSnake = std::list<ALLEGRO_VERTEX>();
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 100, .y = 40, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 110, .y = 40, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 120, .y = 40, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 130, .y = 40, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 140, .y = 40, .color = mSnakeColor});
}

void Snake::Grow()
{
    // Place it out of sight it, it will get position from the last piece
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = -200, .y = -200, .color = mSnakeColor});
}

void Snake::Move(float x, float y)
{
    mDirection.x = 0;
    mDirection.y = 0;

    if(x > 0) mDirection.x = 1;
    if(x < 0) mDirection.x = -1;
    if(y > 0) mDirection.y = 1;
    if(y < 0) mDirection.y = -1;

    float lastX;
    float lastY;
    for (auto it = std::begin(mSnake), first = it, end = std::end(mSnake); it != end; ++it)
    {

        float tempX = it->x;
        float tempY = it->y;
        if(it == first)
        {
            it->x += x;
            it->y += y;
        }
        else
        {
            it->x = lastX;
            it->y = lastY;
        }
        lastX = tempX;
        lastY = tempY;
    }
}

void Snake::Draw()
{
    for(auto &part : mSnake)
    {
        al_draw_filled_rectangle(part.x, part.y, part.x + mSnakeWidth, part.y + mSnakeWidth, part.color);
    }
}

ALLEGRO_VERTEX &Snake::GetPosition()
{
    return mSnake.front();
}

ALLEGRO_VERTEX &Snake::GetDirection()
{
    return mDirection;
}

bool Snake::CheckCollision()
{
    auto &head = mSnake.front();
    for (auto it = std::begin(mSnake), first = it, end = std::end(mSnake); it != end; ++it)
    {
        if(it == first) continue;

        if(it->x == head.x && it->y == head.y)
            return true;
    }
    return false;
}

