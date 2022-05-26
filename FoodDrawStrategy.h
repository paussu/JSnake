#pragma once

#include <allegro5/allegro_primitives.h>

class FoodDrawStrategy
{
 public:
    virtual ~FoodDrawStrategy(){}

    virtual void Draw(const ALLEGRO_VERTEX &vertex, float foodSize) = 0;
};


class PlainFood : public FoodDrawStrategy
{
 public:
    void Draw(const ALLEGRO_VERTEX &vertex, float foodSize) override
    {
        al_draw_filled_rectangle(vertex.x, vertex.y, vertex.x + foodSize, vertex.y + foodSize, vertex.color);
    }
};

class SpriteFood : public FoodDrawStrategy
{
 public:
    SpriteFood()
    {
        mSprite = al_load_bitmap("../Assets/apple.png");
    }

    ~SpriteFood()
    {
        al_destroy_bitmap(mSprite);
    }

    void Draw(const ALLEGRO_VERTEX &vertex, float foodSize) override
    {
        al_draw_rotated_bitmap(mSprite, foodSize / 2, foodSize / 2, vertex.x, vertex.y, 0, 0);
    }

 private:
    ALLEGRO_BITMAP* mSprite;
};

