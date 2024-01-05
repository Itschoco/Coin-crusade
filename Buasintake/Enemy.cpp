#include "Enemy.h"
#include "raymath.h"
#include <cstdlib>

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float shieldAmount)
	: shield(shieldAmount) {
	worldPos = pos;
	texture = idle_texture;
	idle = idle_texture;
	run = run_texture;
	width = static_cast<float>(texture.width) / maxFrames;
	height = static_cast<float>(texture.height);
	speed = 3.5f;
}


void Enemy::tick(float deltaTime) {
	if (!getAlive()) return; // Check if the enemy is alive

	// Determine movement direction towards the target
	velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
	if (Vector2Length(velocity) < radius) velocity = {}; // Reset velocity if close enough

	// Store the current position for potential reverting
	Vector2 originalPos = worldPos;

	// Update the enemy's position based on velocity and update texture accordingly
	BaseCharacter::tick(deltaTime);

	// Check collision with the target and inflict damage
	if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec())) {
		// If there's a collision, revert the position to the original
		worldPos = originalPos;
		target->takeDamage(damagePerSec * deltaTime);
	}
}


// Calculate and return the enemy's screen position relative to the target
Vector2 Enemy::getScreenPos() {
	return Vector2Subtract(worldPos, target->getWorldPos());
}


void Enemy::UpdateEnemySpawn(std::vector<Enemy*>& enemies, Character* target, Map& gameMap, int windowWidth, int windowHeight) {
	static double lastSpawnTime = GetTime(); // Track time for spawning enemies
	double currentTime = GetTime();

	// Check if it's time to spawn new enemies (every 5 seconds for testing)
	if (currentTime - lastSpawnTime >= 5.0) {
		int numEnemiesToSpawn = GetRandomValue(5, 20); // Randomly choose between 5 to 20 enemies

		for (int i = 0; i < numEnemiesToSpawn; ++i) {
			Vector2 spawnPosition;
			bool collisionDetected = true;
			int maxAttempts = 20;
			int attempts = 0;

			// Attempt to find a spawn location with no collisions within the specified attempts limit
			while (collisionDetected && attempts < maxAttempts) {
				// Calculate map boundaries based on window size
				float mapWidth = gameMap.GetWidth() * gameMap.GetScale();
				float mapHeight = gameMap.GetHeight() * gameMap.GetScale();
				float minX = windowWidth / 2.0f;
				float minY = windowHeight / 2.0f;
				float maxX = mapWidth - windowWidth / 2.0f;
				float maxY = mapHeight - windowHeight / 2.0f;

				// Randomly spawn enemies within expanded map boundaries
				spawnPosition = {
					static_cast<float>(GetRandomValue(static_cast<int>(minX), static_cast<int>(maxX))),
					static_cast<float>(GetRandomValue(static_cast<int>(minY), static_cast<int>(maxY)))
				};

				// Check for collisions with existing enemies
				collisionDetected = false;
				for (auto enemy : enemies) {
					if (CheckCollisionCircles(spawnPosition, 20.0f, enemy->getWorldPos(), 20.0f)) {
						collisionDetected = true;
						break;
					}
				}

				attempts++;
			}

			if (!collisionDetected) {
				// Randomly choose between goblin and slime
				int enemyType = GetRandomValue(0, 1); // 0 for goblin, 1 for slime
				float randomShield = static_cast<float>(GetRandomValue(0, 10));// Random shield amount

				// Load textures based on enemy type
				Texture2D idleTexture, runTexture;
				if (enemyType == 0) {
					idleTexture = LoadTexture("characters/goblin_idle_spritesheet.png");
					runTexture = LoadTexture("characters/goblin_run_spritesheet.png");
				}
				else {
					idleTexture = LoadTexture("characters/slime_idle_spritesheet.png");
					runTexture = LoadTexture("characters/slime_run_spritesheet.png");
				}

				// Create a new enemy with spawn location and shield amount
				Enemy* newEnemy = new Enemy(spawnPosition, idleTexture, runTexture, randomShield);
				newEnemy->setTarget(target);
				newEnemy->increaseShield(randomShield); // Increase shield for the enemy based on the shield property set during creation

				// If no collision, add the enemy to the vector
				enemies.push_back(newEnemy);
			}
		}

		lastSpawnTime = currentTime; // Update last spawn time
	}

	// Remove enemies that are no longer alive
	auto it = std::remove_if(enemies.begin(), enemies.end(), [](Enemy* enemy) {
		if (!enemy->getAlive()) {
			UnloadTexture(enemy->getIdleTexture()); // Unload textures when an enemy dies
			UnloadTexture(enemy->getRunTexture());
			delete enemy; // Delete the enemy object
			return true; // Mark for removal
		}
		return false;
		});

	enemies.erase(it, enemies.end()); // Erase enemies that are no longer alive
}
