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
   [[nodiscard]] bool CheckCollision() const;
   [[nodiscard]] bool IsInside(const ALLEGRO_VERTEX &position) const;

   ALLEGRO_VERTEX &GetPosition();
   ALLEGRO_VERTEX &GetDirection();
   [[nodiscard]] float GetSize() const;

 private:
   float mSnakeWidth = 16.0f;
   ALLEGRO_COLOR mSnakeColor{.r = 255, .g = 0, .b = 0, .a = 255};
   ALLEGRO_VERTEX mDirection{.x = -1, .y = 0};
   ALLEGRO_VERTEX mPreviousDirection{.x = -1, .y = 0};

   std::list<SnakePart> mSnake;
   std::unique_ptr<SnakeDrawStrategy> mDrawStrategy;
};

