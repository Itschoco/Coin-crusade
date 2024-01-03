#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"
#include "BaseCharacter.h"
#include "Enemy.h"
#include "Soundmanager.h"
#include "PointSystem.h"
#include <vector>

class Enemy; // Forward declaration

class Character : public BaseCharacter
{
public:
	Character(int winWidth, int winHeight);
	virtual void tick(float deltaTime) override;
	virtual Vector2 getScreenPos() override;
	Rectangle getWeaponCollisonRec() { return weaponCollisionRec; }
	void attackEnemies(std::vector<Enemy*>& enemies, SoundManager& soundManager, PointSystem& pointSystem);



private:
	int windowWidth{};
	int windowHeight{};
	Texture2D weapon{ LoadTexture("characters/weapon_sword.png") };
	Rectangle weaponCollisionRec{};


};
#endif // !CHARACTER_H
