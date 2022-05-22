//
// Created by jipe on 5/9/21.
//

#include "Food.h"

Food::Food(bool useSprites)
: mFoodSize(16.0f)
{
    if(useSprites)
    {
        mDrawStrategy = std::make_unique<SpriteFood>();
        mFoodSize = 32.0f;
    }
    else
        mDrawStrategy = std::make_unique<PlainFood>();

    mPosition = ALLEGRO_VERTEX{.x = mFoodSize * 7, .y = mFoodSize * 7, .color = ALLEGRO_COLOR{.r = 255, .g = 255, .b = 0, .a = 0}};
}

void Food::Draw() const
{
    mDrawStrategy->Draw(mPosition, mFoodSize);
}

void Food::SetPosition(ALLEGRO_VERTEX newPosition)
{
    mPosition.x = newPosition.x;
    mPosition.y = newPosition.y;
}

bool Food::IsEaten(ALLEGRO_VERTEX &snakePosition) const
{
    return mPosition.x >= snakePosition.x
    && mPosition.x < snakePosition.x + mFoodSize
    && mPosition.y >= snakePosition.y
    && mPosition.y < snakePosition.y + mFoodSize;
}


