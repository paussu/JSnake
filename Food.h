//
// Created by jipe on 5/9/21.
//

#pragma once


#include <allegro_primitives.h>
#include "FoodDrawStrategy.h"
#include <memory>

class Food
{
public:
    Food(bool useSprites = false);
    ~Food() = default;

    void Draw() const;
    void SetPosition(ALLEGRO_VERTEX newPosition);
    bool IsEaten(ALLEGRO_VERTEX &snakePosition) const;
private:
    ALLEGRO_VERTEX mPosition;
    float mFoodSize;
    std::unique_ptr<FoodDrawStrategy> mDrawStrategy;
};


