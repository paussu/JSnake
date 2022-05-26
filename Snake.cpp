//
// Created by jipe on 5/8/21.
//

#include "Snake.h"
#include "PlainSnake.h"
#include "SpriteSnake.h"
#include <ranges>

Snake::Snake(bool useSprites)
: mSnakeWidth(16.0f), mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255}, mDirection{.x = -1, .y = 0}, mPreviousDirection(mDirection)
{
    if (useSprites)
    {
        mDrawStrategy = std::make_unique<SpriteSnake>();
        mSnakeWidth = 32.0f;
    }
    else
        mDrawStrategy = std::make_unique<PlainSnake>();

    mSnake = std::list<SnakePart>();
    mSnake.emplace_back(SnakePart{.vertex = {.x = mSnakeWidth * 4, .y = mSnakeWidth, .color = mSnakeColor}, .isCurve = false, .isInvertedCurve = false, .direction = SnakeDirection::LEFT});
    mSnake.emplace_back(SnakePart{.vertex = {.x = mSnakeWidth * 5, .y = mSnakeWidth, .color = mSnakeColor}, .isCurve = false, .isInvertedCurve = false, .direction = SnakeDirection::LEFT});
    mSnake.emplace_back(SnakePart{.vertex = {.x = mSnakeWidth * 6, .y = mSnakeWidth, .color = mSnakeColor}, .isCurve = false, .isInvertedCurve = false, .direction = SnakeDirection::LEFT});
    mSnake.emplace_back(SnakePart{.vertex = {.x = mSnakeWidth * 7, .y = mSnakeWidth, .color = mSnakeColor}, .isCurve = false, .isInvertedCurve = false, .direction = SnakeDirection::LEFT});
    mSnake.emplace_back(SnakePart{.vertex = {.x = mSnakeWidth * 8, .y = mSnakeWidth, .color = mSnakeColor}, .isCurve = false, .isInvertedCurve = false, .direction = SnakeDirection::LEFT});
}

void Snake::Grow()
{
    // Place it out of sight it, it will get position from the last piece
    mSnake.emplace_back(SnakePart{ALLEGRO_VERTEX{.x = -200, .y = -200, .color = mSnakeColor}, false});
}

void Snake::Move(float x, float y)
{
    SnakeDirection direction;

    mDirection.x = 0;
    mDirection.y = 0;

    if (x > 0)
    {
        mDirection.x = 1;
        direction = SnakeDirection::RIGHT;
    }
    if (x < 0)
    {
        mDirection.x = -1;
        direction = SnakeDirection::LEFT;
    }
    if (y > 0)
    {
        mDirection.y = 1;
        direction = SnakeDirection::DOWN;
    }
    if (y < 0)
    {
        mDirection.y = -1;
        direction = SnakeDirection::UP;
    }

    // We can't go backwards, unless we want the snake to eat itself
    if (mDirection.x == -mPreviousDirection.x && mDirection.y == -mPreviousDirection.y)
    {
        mDirection = mPreviousDirection;
        return;
    }
    // Check if we are turning
    else if (mDirection.x - mPreviousDirection.x != 0 || mDirection.y - mPreviousDirection.y != 0)
    {
        auto curvePart = mSnake.begin();
        curvePart->isCurve = true;

        if (mPreviousDirection.x == 1 && mDirection.y == -1)
            curvePart->isInvertedCurve = true;

        if (mPreviousDirection.y == -1 && mDirection.x == -1)
            curvePart->isInvertedCurve = true;

        if (mPreviousDirection.x == -1 && mDirection.y == 1)
            curvePart->isInvertedCurve = true;

        if (mPreviousDirection.y == 1 && mDirection.x == 1)
            curvePart->isInvertedCurve = true;


        if (mPreviousDirection.x == 1 && mDirection.y == 1)
            curvePart->isInvertedCurve = false;

        if (mPreviousDirection.y == -1 && mDirection.x == 1)
            curvePart->isInvertedCurve = false;

        if (mPreviousDirection.x == -1 && mDirection.y == -1)
            curvePart->isInvertedCurve = false;

        if (mPreviousDirection.y == 1 && mDirection.x == -1)
            curvePart->isInvertedCurve = false;
    }

    float lastX;
    float lastY;

    SnakeDirection lastDirection;
    bool lastCurve;
    bool lastInvertedCurve;
    for (auto it = std::begin(mSnake), first = it, end = std::end(mSnake); it != end; ++it)
    {

        float tempX = it->vertex.x;
        float tempY = it->vertex.y;

        SnakeDirection tempDirection = it->direction;

        bool tempCurve = it->isCurve;
        bool tempInvertedCurve = it->isInvertedCurve;
        if (it == first)
        {
            it->vertex.x += x;
            it->vertex.y += y;

            it->direction = direction;

            it->isCurve = false;
            it->isInvertedCurve = false;
        }
        else
        {
            it->vertex.x = lastX;
            it->vertex.y = lastY;

            it->direction = lastDirection;

            it->isCurve = lastCurve;
            it->isInvertedCurve = lastInvertedCurve;
        }
        lastX = tempX;
        lastY = tempY;

        lastDirection = tempDirection;

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

    return std::any_of(std::next(mSnake.begin()), mSnake.end(), [&head](const SnakePart &snakePart)
    {
        return snakePart.vertex.x == head.vertex.x && snakePart.vertex.y == head.vertex.y;
    });
}

bool Snake::IsInside(ALLEGRO_VERTEX &position)
{
    return std::any_of(mSnake.begin(), mSnake.end(), [&position](const SnakePart &snakePart)
    {
        return snakePart.vertex.x == position.x && snakePart.vertex.y == position.y;
    });
}

float Snake::GetSize()
{
    return mSnakeWidth;
}
