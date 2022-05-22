#include "SpriteSnake.h"
#include <stdio.h>

constexpr float ANGLE_LEFT = ALLEGRO_PI;
constexpr float ANGLE_RIGHT = 0;
constexpr float ANGLE_UP = (3 * ALLEGRO_PI) / 2;
constexpr float ANGLE_DOWN = ALLEGRO_PI / 2;

ALLEGRO_VERTEX previousVertex;

bool operator==(const ALLEGRO_VERTEX &vertex1, const ALLEGRO_VERTEX &vertex2)
{
    return vertex1.color.a == vertex2.color.a 
    && vertex1.color.r == vertex2.color.r
    && vertex1.color.b == vertex2.color.b
    && vertex1.color.g == vertex2.color.g
    && vertex1.u == vertex2.u
    && vertex1.v == vertex2.v
    && vertex1.x == vertex2.x
    && vertex1.y == vertex2.y
    && vertex1.z == vertex2.z;

}

bool operator==(const SnakePart &part1, const SnakePart &part2)
{
    return part1.isCurve == part2.isCurve && part1.vertex == part2.vertex;
}

SpriteSnake::SpriteSnake()
{
    mHeadSprite = al_load_bitmap("../Assets/snake_head.png");
    mBodySprite = al_load_bitmap("../Assets/snake_body.png");
    mCurveSprite = al_load_bitmap("../Assets/snake_curve.png");
    mInvertedCurveSprite = al_load_bitmap("../Assets/snake_curve_inv.png");
    mTailSprite = al_load_bitmap("../Assets/snake_tail.png");
}

SpriteSnake::~SpriteSnake()
{
    al_destroy_bitmap(mHeadSprite);
    al_destroy_bitmap(mBodySprite);
    al_destroy_bitmap(mCurveSprite);
    al_destroy_bitmap(mInvertedCurveSprite);
    al_destroy_bitmap(mTailSprite);
}

void SpriteSnake::Draw(const std::list<SnakePart> &snake, float snakeWidth)
{
    const auto &snakeHead = snake.front();
    const auto &snakeTail = snake.back();

    float angle = 0;
    ALLEGRO_BITMAP *sprite = nullptr;
    for(const auto &snakePart : snake)
    {
        const auto &vertex = snakePart.vertex;
        const auto isCurve = snakePart.isCurve;

        angle = 0;
        if(vertex.u == 1)
            angle = ANGLE_RIGHT;
        else if(vertex.u == -1)
            angle = ANGLE_LEFT;
        else if(vertex.v == 1)
            angle = ANGLE_DOWN;
        else if(vertex.v == -1)
            angle = ANGLE_UP;

        if(snakePart == snake.front())
            sprite = mHeadSprite;
        else if(snakePart == snake.back())
        {
            sprite = mTailSprite;

            auto previousPart = std::prev(snake.end(), 2);
            auto prevVertex = previousPart->vertex;

            if(prevVertex.u == 1)
                angle = ANGLE_RIGHT;
            else if(prevVertex.u == -1)
                angle = ANGLE_LEFT;
            else if(prevVertex.v == 1)
                angle = ANGLE_DOWN;
            else if(prevVertex.v == -1)
                angle = ANGLE_UP;
        }
        else if(isCurve)
        {
            sprite = mCurveSprite;
            if(snakePart.isInvertedCurve)
            {
                sprite = mInvertedCurveSprite;
                angle += ALLEGRO_PI;
            }
        }
        else
            sprite = mBodySprite;

        al_draw_rotated_bitmap(sprite, 16, 16, vertex.x, vertex.y, angle, 0);
    }
}
