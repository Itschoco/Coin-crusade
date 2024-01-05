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
			enemy->takeDamage(15); // Deal damage to the enemy
			soundManager.PlayEnemyDeadSound();

			// Check if the enemy is dead
			if (!enemy->getAlive()) {
				pointSystem.increaseScore(50); // Increase score when enemy is killed
			}
		}
	}
}

// Get the screen position of the character
Vector2 Character::getScreenPos() {
	return Vector2{
		static_cast<float>(windowWidth) / 2.0f - scale * (0.5f * width),
		static_cast<float>(windowHeight) / 2.0f - scale * (0.5f * height)
	};
}


void Character::tick(float deltaTime) {
	if (!getAlive()) return; // Checks if player is alive

	// Handle movement based on key input
	if (IsKeyDown(KEY_A)) velocity.x -= 1.0;
	if (IsKeyDown(KEY_D)) velocity.x += 1.0;
	if (IsKeyDown(KEY_W)) velocity.y -= 1.0;
	if (IsKeyDown(KEY_S)) velocity.y += 1.0;

	BaseCharacter::tick(deltaTime);

	// Set up weapon parameters based on character direction
	Vector2 origin{};
	Vector2 offset{};
	float rotation{};
	if (rightLeft > 0.f) {
		origin = { 0.f, weapon.height * scale };
		offset = { 35.f,55.f };
		rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 35.f : 0.f;
	}
	else {
		origin = { weapon.width * scale , weapon.height * scale };
		offset = { 25.f,55.f };
		rotation = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -35.f : 0.f;
	}

	// Calculate weapon collision rectangle and draw the weapon
	weaponCollisionRec = {
		getScreenPos().x + offset.x - (weapon.width * scale * (1.f - rightLeft)),
		getScreenPos().y + offset.y - weapon.height * scale,
		weapon.width * scale,
		weapon.height * scale
	};
	Rectangle source{ 0.f,0.f,static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height) };
	Rectangle dest{ getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width * scale, weapon.height * scale };
	DrawTexturePro(weapon, source, dest, origin, rotation, WHITE); // Draw weapon texture

}

// Reset the character's attributes to their initial state
void Character::Reset() {
	setAlive(true);
	// Reset the character's position to the starting position or any desired position
	worldPos = Vector2{ static_cast<float>(windowWidth) / 2.0f, static_cast<float>(windowHeight) / 2.0f };
	// Reset health to full
	health = 100.0f;
	// Reset shield 
	shield = 0.0f;
}
