#pragma once

#include <list>
#include <allegro5/allegro_primitives.h>

struct SnakePart
{
    ALLEGRO_VERTEX vertex;
    bool isCurve;
    bool isInvertedCurve = false;
};

class SnakeDrawStrategy
{
public:
    virtual ~SnakeDrawStrategy(){}

    virtual void Draw(const std::list<SnakePart> &snake, float snakeWidth) = 0;
};