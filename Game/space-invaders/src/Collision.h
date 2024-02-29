#pragma once
#include "GameObject.h"

class Collision
{
public:
    static bool IsColliding(const GameObject& A, const GameObject& B)
    {
        const float wholeXA = A.Position.x + A.Size.x;
        const float wholeXB = B.Position.x + B.Size.x;
        const bool collisionX = wholeXA >= B.Position.x && wholeXB >= A.Position.x;

        const float wholeYA = A.Position.y + A.Size.y;
        const float wholeYB = B.Position.y + B.Size.y;
        const bool collisionY = wholeYA >= B.Position.y && wholeYB >= A.Position.y;

        return collisionX && collisionY;
    }
};
