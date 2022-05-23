#pragma once

#include <list>
#include <allegro5/allegro_primitives.h>

enum class SnakeDirection
{
    LEFT,
    RIGHT,
    DOWN,
    UP
};

struct SnakePart
{
    ALLEGRO_VERTEX vertex;
    bool isCurve;
    bool isInvertedCurve;
    SnakeDirection direction;
};

class SnakeDrawStrategy
{
public:
    virtual ~SnakeDrawStrategy(){}

    virtual void Draw(const std::list<SnakePart> &snake, float snakeWidth) = 0;
};