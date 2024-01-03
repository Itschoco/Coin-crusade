#include "Enemy.h"
#include "raymath.h"
#include <cstdlib> // Include this for random number generation




Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture, float shieldAmount)
	: shield(shieldAmount) {
	worldPos = pos;
	texture = idle_texture;
	idle = idle_texture;
	run = run_texture;
	width = texture.width / maxFrames;
	height = texture.height;
	speed = 3.5f;
}

void Enemy::tick(float deltaTime) {
	if (!getAlive()) return;

	// get toTarget
	velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
	if (Vector2Length(velocity) < radius) velocity = {};
	BaseCharacter::tick(deltaTime);

	if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec())) {
		target->takeDamage(damagePerSec * deltaTime);
	}
}
Vector2 Enemy::getScreenPos()
{
	return Vector2Subtract(worldPos, target->getWorldPos());
}







void Enemy::UpdateEnemySpawn(std::vector<Enemy*>& enemies, Character* target, Map& gameMap, int windowWidth, int windowHeight) {
	// Track time for spawning enemies
	static double lastSpawnTime = GetTime();

	// Check if it's time to spawn new enemies (every 5 seconds for testing)
	double currentTime = GetTime();
	if (currentTime - lastSpawnTime >= 5.0) // Check if 5 seconds have passed for testing
	{
		int numEnemiesToSpawn = GetRandomValue(10, 50); // Randomly choose between 10 to 20 enemies

		for (int i = 0; i < numEnemiesToSpawn; ++i)
		{
			Vector2 spawnPosition;
			bool collisionDetected = true;
			int maxAttempts = 20;
			int attempts = 0;

			// Attempt to find a spawn location with no collisions within the specified attempts limit
			while (collisionDetected && attempts < maxAttempts)
			{
				// Get scaled map dimensions from the Map instance
				float mapWidth = gameMap.GetWidth() * gameMap.GetScale();
				float mapHeight = gameMap.GetHeight() * gameMap.GetScale();

				// Calculate the map boundaries based on the window size
				float minX = windowWidth / 2.0f;
				float minY = windowHeight / 2.0f;
				float maxX = mapWidth - windowWidth / 2.0f;
				float maxY = mapHeight - windowHeight / 2.0f;

				// Randomly spawn enemies within the expanded map boundaries
				spawnPosition = {
					static_cast<float>(GetRandomValue(static_cast<int>(minX), static_cast<int>(maxX))),
					static_cast<float>(GetRandomValue(static_cast<int>(minY), static_cast<int>(maxY)))
				};

				// Check for collisions with existing enemies
				collisionDetected = false;
				for (auto enemy : enemies)
				{
					if (CheckCollisionCircles(spawnPosition, 20.0f, enemy->getWorldPos(), 20.0f))
					{
						collisionDetected = true;
						break;
					}
				}

				attempts++;
			}

			if (!collisionDetected)
			{
				// Randomly choose between goblin and slime
				int enemyType = GetRandomValue(0, 1); // 0 for goblin, 1 for slime
				// Create a random shield for some enemies
				float randomShield = GetRandomValue(0, 50);

				// Load textures based on enemy type
				Texture2D idleTexture, runTexture;
				if (enemyType == 0)
				{
					idleTexture = LoadTexture("characters/goblin_idle_spritesheet.png");
					runTexture = LoadTexture("characters/goblin_run_spritesheet.png");
				}
				else
				{
					idleTexture = LoadTexture("characters/slime_idle_spritesheet.png");
					runTexture = LoadTexture("characters/slime_run_spritesheet.png");
				}

				// Create a new enemy with the selected spawn location and shield amount
				Enemy* newEnemy = new Enemy(spawnPosition, idleTexture, runTexture, randomShield);
				newEnemy->setTarget(target);
				newEnemy->increaseShield(randomShield); // Increase shield for the enemy based on the shield property set during creation

				// If no collision, add the enemy to the vector
				enemies.push_back(newEnemy);
			}
		}

		lastSpawnTime = currentTime; // Update the last spawn time
	}

	// Check if any enemies need to be destroyed (optional)
	auto it = std::remove_if(enemies.begin(), enemies.end(), [](Enemy* enemy) {
		if (!enemy->getAlive()) {
			// Unload textures when an enemy dies
			UnloadTexture(enemy->getIdleTexture());
			UnloadTexture(enemy->getRunTexture());
			delete enemy; // Delete the enemy object
			return true; // Mark for removal
		}
		return false;
		});

	// Erase enemies that are no longer alive
	enemies.erase(it, enemies.end());
}
