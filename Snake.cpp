//
// Created by jipe on 5/8/21.
//

#include <ranges>
#include "Snake.h"
#include "PlainSnake.h"
#include "SpriteSnake.h"

Snake::Snake(bool useSprites)
: mSnakeWidth(16.0f), mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255}, mDirection{.x = -1, .y = 0}, mPreviousDirection(mDirection)
{
    if(useSprites)
    {
        mDrawStrategy = std::make_unique<SpriteSnake>();
        mSnakeWidth = 32.0f;
    }
    else
        mDrawStrategy = std::make_unique<PlainSnake>();

    mSnake = std::list<SnakePart>();
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = mSnakeWidth * 4, .y = mSnakeWidth, .u = -1, .v = 0, .color = mSnakeColor}, false});
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = mSnakeWidth * 5, .y = mSnakeWidth, .u = -1, .v = 0, .color = mSnakeColor}, false});
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = mSnakeWidth * 6, .y = mSnakeWidth, .u = -1, .v = 0, .color = mSnakeColor}, false});
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = mSnakeWidth * 7, .y = mSnakeWidth, .u = -1, .v = 0, .color = mSnakeColor}, false});
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = mSnakeWidth * 8, .y = mSnakeWidth, .u = -1, .v = 0, .color = mSnakeColor}, false});
}

void Snake::Grow()
{
    // Place it out of sight it, it will get position from the last piece
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = -200, .y = -200, .color = mSnakeColor}, false});
}

void Snake::Move(float x, float y)
{
    mDirection.x = 0;
    mDirection.y = 0;

    if(x > 0)
        mDirection.x = 1;
    if(x < 0)
        mDirection.x = -1;
    if(y > 0)
        mDirection.y = 1;
    if(y < 0)
        mDirection.y = -1;

    // We can't go backwards, unless we want the snake to eat itself
    if(mDirection.x == -mPreviousDirection.x && mDirection.y == -mPreviousDirection.y)
    {
        mDirection = mPreviousDirection;
        return;
    }
    // Check if we are turning
    else if(mDirection.x - mPreviousDirection.x != 0 || mDirection.y - mPreviousDirection.y != 0)
    {
        auto curvePart = mSnake.begin();
        curvePart->isCurve = true;

        if(mPreviousDirection.x == 1 && mDirection.y == -1)
            curvePart->isInvertedCurve = true;

        if(mPreviousDirection.y == -1 && mDirection.x == -1)
            curvePart->isInvertedCurve = true;

        if(mPreviousDirection.x == -1 && mDirection.y == 1)
            curvePart->isInvertedCurve = true;

        if(mPreviousDirection.y == 1 && mDirection.x == 1)
            curvePart->isInvertedCurve = true;


        if(mPreviousDirection.x == 1 && mDirection.y == 1)
            curvePart->isInvertedCurve = false;

        if(mPreviousDirection.y == -1 && mDirection.x == 1)
            curvePart->isInvertedCurve = false;

        if(mPreviousDirection.x == -1 && mDirection.y == -1)
            curvePart->isInvertedCurve = false;

        if(mPreviousDirection.y == 1 && mDirection.x == -1)
            curvePart->isInvertedCurve = false;
    }

    float lastX;
    float lastY;

    float lastU;
    float lastV;
    bool lastCurve;
    bool lastInvertedCurve;
    for (auto it = std::begin(mSnake), first = it, end = std::end(mSnake); it != end; ++it)
    {

        float tempX = it->vertex.x;
        float tempY = it->vertex.y;

        float tempU = it->vertex.u;
        float tempV = it->vertex.v;

        bool tempCurve = it->isCurve;
        bool tempInvertedCurve = it->isInvertedCurve;
        if(it == first)
        {
            it->vertex.x += x;
            it->vertex.y += y;

            it->vertex.u = mDirection.x;
            it->vertex.v = mDirection.y;

            it->isCurve = false;
            it->isInvertedCurve = false;
        }
        else
        {
            it->vertex.x = lastX;
            it->vertex.y = lastY;

            it->vertex.u = lastU;
            it->vertex.v = lastV;

            it->isCurve = lastCurve;
            it->isInvertedCurve = lastInvertedCurve;
        }
        lastX = tempX;
        lastY = tempY;

        lastU = tempU;
        lastV = tempV;

        lastCurve = tempCurve;
        lastInvertedCurve = tempInvertedCurve;
    }

    mPreviousDirection = mDirection;
}

void Snake::Draw()
{
    mDrawStrategy->Draw(mSnake, mSnakeWidth);
}

ALLEGRO_VERTEX &Snake::GetPosition()
{
    return mSnake.front().vertex;
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

        if(it->vertex.x == head.vertex.x && it->vertex.y == head.vertex.y)
            return true;
    }
    return false;
}

bool Snake::IsInside(ALLEGRO_VERTEX &position)
{
    for (auto it = std::begin(mSnake), end = std::end(mSnake); it != end; ++it)
    {
        if(it->vertex.x == position.x && it->vertex.y == position.y)
            return true;
    }
    return false;
}

float Snake::GetSize()
{
    return mSnakeWidth;
}