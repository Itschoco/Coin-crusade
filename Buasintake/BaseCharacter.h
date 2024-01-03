
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

#pragma once
#include "raylib.h"


class BaseCharacter {
public:
	BaseCharacter();

	// Returns the character's world position
	Vector2 getWorldPos() const { return worldPos; };

	// Reverts the character's movement
	void undoMovement();

	// Retrieves the collision rectangle
	Rectangle getCollisionRec();

	// Updates character logic per frame
	virtual void tick(float deltaTime);

	//Get the character's screen position
	virtual Vector2 getScreenPos() = 0;

	// Gets the alive status of the character
	bool getAlive() { return alive; }

	// Sets the alive status of the character
	void setAlive(bool isAlive) { alive = isAlive; }

	float getHealth() const { return health; }
	void takeDamage(float damage);
	float getShield() const { return shield; }
	void increaseShield(float amount) { shield += amount; }

private:
	bool alive{ true }; // Indicates if the character is alive

protected:
	// Textures for character animations
	Texture2D texture{ LoadTexture("characters/knight_idle_spritesheet.png") };
	Texture2D idle{ LoadTexture("characters/knight_idle_spritesheet.png") };
	Texture2D run{ LoadTexture("characters/knight_run_spritesheet.png") };

	// Character position and movement attributes
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

	// Character health and shield attributes
	float health{ 100.0f };
	float shield{ 0.0f };
};

#endif // !BASE_CHARACTER_H


