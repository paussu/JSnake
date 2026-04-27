#pragma once


#include <allegro5/allegro_primitives.h>
#include "FoodDrawStrategy.h"
#include <memory>

class Food
{
 public:
    explicit Food(bool useSprites = false);
    ~Food() = default;

    void Draw() const;
    void SetPosition(ALLEGRO_VERTEX newPosition);
    bool IsEaten(const ALLEGRO_VERTEX &snakePosition) const;
 private:
   ALLEGRO_VERTEX mPosition{.x = 112.0f, .y = 112.0f, .color = {.r = 255, .g = 255, .b = 0, .a = 0}};
   float mFoodSize = 16.0f;
    std::unique_ptr<FoodDrawStrategy> mDrawStrategy;
};


