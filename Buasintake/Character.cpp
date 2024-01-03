#include "Character.h"
#include "raymath.h"




Character::Character(int winWidth, int winHeight) :
	windowWidth(winWidth),
	windowHeight(winHeight)
{
	width = static_cast<float>(texture.width) / maxFrames;
	height = static_cast<float>(texture.height);



}


void Character::attackEnemies(std::vector<Enemy*>& enemies, SoundManager& soundManager, PointSystem& pointSystem) {
	soundManager.PlayHitSound();
	for (auto enemy : enemies) {
		if (CheckCollisionRecs(enemy->getCollisionRec(), getWeaponCollisonRec())) {
			enemy->takeDamage(15); // Deal 10 damage to the enemy
			soundManager.PlayEnemyDeadSound();

			// Check if the enemy is dead
			if (!enemy->getAlive()) {
				pointSystem.increaseScore(100); // Increase score when enemy is killed
			}
		}
	}
}


Vector2 Character::getScreenPos()
{
	return Vector2{

		static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
		static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)

	};
}

void Character::tick(float deltaTime)
{
	if (!getAlive()) return;

	if (IsKeyDown(KEY_A))velocity.x -= 1.0;
	if (IsKeyDown(KEY_D))velocity.x += 1.0;
	if (IsKeyDown(KEY_W))velocity.y -= 1.0;
	if (IsKeyDown(KEY_S))velocity.y += 1.0;



	BaseCharacter::tick(deltaTime);

	Vector2 origin{};
	Vector2 offset{};
	float rotation{};
	if (rightLeft > 0.f)
	{
		origin = { 0.f, weapon.height * scale };
		offset = { 35.f,55.f };
		weaponCollisionRec = {
			getScreenPos().x + offset.x,
			getScreenPos().y + offset.y - weapon.height * scale,
			weapon.width * scale,
			weapon.height * scale
		};
		rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
	}
	else
	{
		origin = { weapon.width * scale , weapon.height * scale };
		offset = { 25.f,55.f };
		weaponCollisionRec = {
			getScreenPos().x + offset.x - weapon.width * scale ,
			getScreenPos().y + offset.y - weapon.height * scale  ,
			weapon.width * scale,
			weapon.height * scale
		};
		rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;

	}

	//draw sword
	Rectangle source{ 0.f,0.f,static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height) };
	Rectangle dest{ getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale };
	DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);
	DrawRectangleLines(
		static_cast<int>(weaponCollisionRec.x),
		static_cast<int>(weaponCollisionRec.y),
		static_cast<int>(weaponCollisionRec.width),
		static_cast<int>(weaponCollisionRec.height),
		RED
	);



}

