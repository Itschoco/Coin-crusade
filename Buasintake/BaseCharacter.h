#pragma once
#include "raylib.h"
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

class BaseCharacter {
public:
	BaseCharacter();
	Vector2 getWorldPos() const { return worldPos; };
	void undoMovement();
	Rectangle getCollisionRec();
	virtual void tick(float deltaTime);
	virtual Vector2 getScreenPos() = 0;
	bool getAlive() { return alive; }
	void setAlive(bool isAlive) { alive = isAlive; }
	float getHealth() const { return health; }
	void takeDamage(float damage);
	float getShield() const { return shield; }
	void increaseShield(float amount) { shield += amount; }

private:
	bool alive{ true };

protected:
	Texture2D texture{ LoadTexture("characters/knight_idle_spritesheet.png") };
	Texture2D idle{ LoadTexture("characters/knight_idle_spritesheet.png") };
	Texture2D run{ LoadTexture("characters/knight_run_spritesheet.png") };
	Vector2 worldPos{};
	Vector2 worldPosLastFrame{};
	float runningTime{};
	int frame{};
	const int maxFrames{ 6 };
	const float updateTime{ 1.f / 12.f };
	float speed{ 4.f };
	float rightLeft{ 1.f };
	float width{};
	float height{};
	float scale{ 4.f };
	Vector2 velocity{};
	float health{ 100.0f };
	float shield{ 0.0f };
};

#endif // !BASE_CHARACTER_H