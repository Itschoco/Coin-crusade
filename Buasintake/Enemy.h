#pragma once
#ifndef ENEMY_H
#define	ENEMY_H


#include "raylib.h"
#include "Character.h"
#include "Map.h"
#include "BaseCharacter.h"
#include "vector"

class Character; // Forward declaration


// Enemy.h

class Enemy : public BaseCharacter {
public:
	Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float shieldAmount);
	virtual void tick(float deltaTime) override;
	void setTarget(Character* character) { target = character; }
	virtual Vector2 getScreenPos() override;
	static void UpdateEnemySpawn(std::vector<Enemy*>& enemies, Character* target, Map& gameMap, int windowWidth, int windowHeight);


private:
	Character* target;
	float damagePerSec{ 10.f };
	float radius{ 26.f };
	Texture2D getIdleTexture() const { return idle; }
	Texture2D getRunTexture() const { return run; }
	float shield{ 0.0f }; // Shield property
};

#endif // !ENEMY_H
