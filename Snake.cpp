//
// Created by jipe on 5/8/21.
//

#include "Snake.h"

Snake::Snake()
: mSnakeWidth(10.0f), mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255}, mDirection{.x = -1, .y = 0}, mPreviousDirection{.x = -1, .y = 0}
{
    mSnake = std::list<ALLEGRO_VERTEX>();
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 100, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 110, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 120, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 130, .y = 10, .color = mSnakeColor});
    mSnake.emplace_back(ALLEGRO_VERTEX{.x = 140, .y = 10, .color = mSnakeColor});
}

void Snake::Grow()
{
    const auto &head = mSnake.front();
    mSnake.emplace_front(ALLEGRO_VERTEX{.x = head.x + (mSnakeWidth * mDirection.x), .y = head.y + (mSnakeWidth * mDirection.y), .color = mSnakeColor});
    mDirection.x = 0;
    mDirection.y = 0;
}

void Snake::Move(float x, float y)
{
    if(x > 0) mDirection.x = 1;
    if(x < 0) mDirection.x = -1;
    if(y > 0) mDirection.y = 1;
    if(y < 0) mDirection.y = -1;

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

    mPreviousDirection.x = mDirection.x;
    mPreviousDirection.y = mDirection.y;
    mDirection.x = 0;
    mDirection.y = 0;
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

ALLEGRO_VERTEX &Snake::GetPreviousDirection()
{
    return mPreviousDirection;
}

bool Snake::CheckCollision()
{
    return false;
}

