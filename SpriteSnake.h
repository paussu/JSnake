#pragma once

#include "SnakeDrawStrategy.h"

class SpriteSnake : public SnakeDrawStrategy
{
public:
    SpriteSnake();
    ~SpriteSnake();

    void Draw(const std::list<SnakePart> &snake, float snakeWidth) override;

private:
    ALLEGRO_BITMAP* mHeadSprite;
    ALLEGRO_BITMAP* mBodySprite;
    ALLEGRO_BITMAP* mCurveSprite;
    ALLEGRO_BITMAP* mInvertedCurveSprite;
    ALLEGRO_BITMAP* mTailSprite;
};