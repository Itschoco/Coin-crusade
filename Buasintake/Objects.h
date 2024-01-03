#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"
#include "GameObject.h"
#include "raymath.h"

class Coin : public GameObject {
public:
    Texture2D texture; // Add texture for coin
    int frameCount = 5; // Set frame count for coin texture
    int currentFrame = 0; // Track current frame for animation
    float updateTime{}; // Update time for animation frames

    Coin(Vector2 pos) : GameObject(pos, 10.0f) {
        texture = LoadTexture("objects/coin.png");
    }

    void Draw(const Vector2& screenPos) const override {
        Rectangle sourceRect{
            static_cast<float>(currentFrame * texture.width / frameCount),
            0.0f,
            static_cast<float>(texture.width / frameCount),
            static_cast<float>(texture.height)
        };
        Rectangle destRect{
            screenPos.x - radius,
            screenPos.y - radius,
            2.0f * radius,
            2.0f * radius
        };
        DrawTexturePro(texture, sourceRect, destRect, Vector2{ radius, radius }, 0.0f, WHITE);
    }

};

class HealthBottle : public GameObject {
public:
    Texture2D texture; // Add texture for health bottle
    int frameCount = 4; // Set frame count for health bottle texture
    int currentFrame = 0; // Track current frame for animation
    float updateTime{}; // Update time for animation frames

    HealthBottle(Vector2 pos) : GameObject(pos, 10.0f) {
        texture = LoadTexture("objects/health.png");
    }

    void Draw(const Vector2& screenPos) const override {
        Rectangle sourceRect{
            static_cast<float>(currentFrame * texture.width / frameCount),
            0.0f,
            static_cast<float>(texture.width / frameCount),
            static_cast<float>(texture.height)
        };
        Rectangle destRect{
            screenPos.x - radius,
            screenPos.y - radius,
            2.0f * radius,
            2.0f * radius
        };
        DrawTexturePro(texture, sourceRect, destRect, Vector2{ radius, radius }, 0.0f, WHITE);
    }

};

class ShieldBottle : public GameObject {
public:
    Texture2D texture; // Add texture for shield bottle
    int frameCount = 4; // Set frame count for shield bottle texture
    int currentFrame = 0; // Track current frame for animation
    float updateTime{}; // Update time for animation frames

    ShieldBottle(Vector2 pos) : GameObject(pos, 10.0f) {
        texture = LoadTexture("objects/shield.png");
    }

    void Draw(const Vector2& screenPos) const override {
        Rectangle sourceRect{
            static_cast<float>(currentFrame * texture.width / frameCount),
            0.0f,
            static_cast<float>(texture.width / frameCount),
            static_cast<float>(texture.height)
        };
        Rectangle destRect{
            screenPos.x - radius,
            screenPos.y - radius,
            2.0f * radius,
            2.0f * radius
        };
        DrawTexturePro(texture, sourceRect, destRect, Vector2{ radius, radius }, 0.0f, WHITE);
    }

};

#endif // !OBJECT_H
