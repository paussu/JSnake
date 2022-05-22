#include "PlainSnake.h"

void PlainSnake::Draw(const std::list<SnakePart> &snake, float snakeWidth)
{
    for(const auto &part : snake)
    {
        const auto &vertex = part.vertex;
        al_draw_filled_rectangle(vertex.x, vertex.y, vertex.x + snakeWidth, vertex.y + snakeWidth, vertex.color);
    }
}