#pragma once

#include <allegro5/allegro_primitives.h>
#include <list>

enum class SnakeDirection
{
    LEFT,
    RIGHT,
    DOWN,
    UP
};

struct SnakePart
{
    ALLEGRO_VERTEX vertex{};
    bool isCurve = false;
    bool isInvertedCurve = false;
    SnakeDirection direction = SnakeDirection::LEFT;
};

class SnakeDrawStrategy
{
 public:
    virtual ~SnakeDrawStrategy() = default;

    virtual void Draw(const std::list<SnakePart> &snake, float snakeWidth) = 0;
};

