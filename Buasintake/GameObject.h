#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "raymath.h"

class GameObject {
public:
    Vector2 worldPos;
    float radius;

    GameObject(Vector2 pos, float r) : worldPos(pos), radius(r) {}

    virtual void Draw(const Vector2& screenPos) const {
        DrawCircleV(screenPos, radius, GRAY);
    }

    float deleteTime = 0.0f; // Time since object creation
};

#endif // !GAME_OBJECT_H
