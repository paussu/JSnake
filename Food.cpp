//
// Created by jipe on 5/9/21.
//

#include "Food.h"

Food::Food()
: mPosition(ALLEGRO_VERTEX{.x = 200, .y = 200, .color = ALLEGRO_COLOR{.r = 255, .g = 255, .b = 0, .a = 0}})
{

}

void Food::Draw() const
{
    al_draw_filled_rectangle(mPosition.x, mPosition.y, mPosition.x + 10, mPosition.y + 10, mPosition.color);
}

void Food::SetPosition(ALLEGRO_VERTEX newPosition)
{
    mPosition.x = newPosition.x;
    mPosition.y = newPosition.y;
}

bool Food::IsEaten(ALLEGRO_VERTEX &snakePosition) const
{
    return mPosition.x >= snakePosition.x && mPosition.x <= snakePosition.x + 10 && mPosition.y >= snakePosition.y && mPosition.y <= snakePosition.y + 10;
}


