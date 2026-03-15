#pragma once

#include "SnakeDrawStrategy.h"

class PlainSnake : public SnakeDrawStrategy
{
 public:
    void Draw(const std::list<SnakePart> &snake, float snakeWidth) override;
};

