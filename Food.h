//
// Created by jipe on 5/9/21.
//

#pragma once


#include <allegro_primitives.h>

class Food
{
public:
    Food();
    ~Food() = default;

    void Draw() const;
    void SetPosition(ALLEGRO_VERTEX newPosition);
    bool IsEaten(ALLEGRO_VERTEX &snakePosition) const;
private:
    ALLEGRO_VERTEX mPosition;
};


