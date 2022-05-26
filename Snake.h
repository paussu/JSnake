//
// Created by jipe on 5/8/21.
//

#pragma once

#include <allegro5/allegro_primitives.h>
#include <list>
#include <memory>

#include "SnakeDrawStrategy.h"

class Snake
{
 public:
    explicit Snake(bool useSprites = false);
    ~Snake() = default;

    void Grow();
    void Move(float x, float y);
    void Draw();
    bool CheckCollision();
    bool IsInside(ALLEGRO_VERTEX &position);

    ALLEGRO_VERTEX &GetPosition();
    ALLEGRO_VERTEX &GetDirection();
    float GetSize();

 private:
    std::list<SnakePart> mSnake;
    float mSnakeWidth;
    ALLEGRO_COLOR mSnakeColor;
    ALLEGRO_VERTEX mDirection;
    ALLEGRO_VERTEX mPreviousDirection;

    std::unique_ptr<SnakeDrawStrategy> mDrawStrategy;
};

