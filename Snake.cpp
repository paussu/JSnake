//
// Created by jipe on 5/8/21.
//

#include "Snake.h"

Snake::Snake()
: mSnakeWidth(10.0f), mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255}, mSnakeDirection{.x = 0, .y = 0}
{
    mSnake = std::list<ALLEGRO_VERTEX>();
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 10, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 20, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 30, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 40, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 50, .y = 10, .color = mSnakeColor});
}

void Snake::Grow()
{
    const auto &head = mSnake.front();
    mSnake.emplace_front(ALLEGRO_VERTEX{.x = head.x + (mSnakeWidth * mSnakeDirection.x), .y = head.y + (mSnakeWidth * mSnakeDirection.y), .color = mSnakeColor});
    mSnakeDirection.x = 0;
    mSnakeDirection.y = 0;
}

void Snake::Move(float x, float y)
{
    if(x > 0) mSnakeDirection.x = 1;
    if(x < 0) mSnakeDirection.x = 0;
    if(y > 0) mSnakeDirection.y = 1;
    if(y < 0) mSnakeDirection.y = 0;

    float lastX;
    float lastY;
    for(auto &part : mSnake)
    {

        float tempX = part.x;
        float tempY = part.y;
        if(part.x == mSnake.front().x && part.y == mSnake.front().y)
        {
            part.x += x;
            part.y += y;
        }
        else
        {
            part.x = lastX;
            part.y = lastY;
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

