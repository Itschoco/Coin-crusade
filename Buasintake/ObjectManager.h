#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include "Objects.h"
#include "GameObject.h"
#include "Map.h"
#include "Character.h"
#include "PointSystem.h"
#include "SoundManager.h"

class ObjectManager {
public:
	std::vector<GameObject*> objects;

	//Update the constructor to accept PointSystem by reference
	ObjectManager(Map& map, Character& knightObj, PointSystem& pointSystemRef, SoundManager& soundManagerRef)
		: gameMap(map), knight(knightObj), pointSystem(pointSystemRef), soundManager(soundManagerRef) {}

	float spawnInterval = 1.0f; // Time between spawns in seconds
	float elapsedTime = 0.0f; // Time elapsed since last spawn

	void Update(float deltaTime, const Vector2& playerPos, int windowWidth, int windowHeight) {
		elapsedTime += deltaTime;

		// Check for object deletion first
		auto it = objects.begin();
		while (it != objects.end()) {
			GameObject* object = *it;
			object->deleteTime += deltaTime;

			// Check if the object exceeds the time limit
			if (object->deleteTime >= 20.0f) {
				// Free the texture associated with the object before deleting it
				if (Coin* coin = dynamic_cast<Coin*>(object)) {
					UnloadTexture(coin->texture);
				}
				else if (HealthBottle* healthBottle = dynamic_cast<HealthBottle*>(object)) {
					UnloadTexture(healthBottle->texture);
				}
				else if (ShieldBottle* shieldBottle = dynamic_cast<ShieldBottle*>(object)) {
					UnloadTexture(shieldBottle->texture);
				}

				delete object;
				it = objects.erase(it);
			}
			else {
				++it;
			}
		}

		if (elapsedTime >= spawnInterval) {
			elapsedTime -= spawnInterval; // Reduce the elapsed time by the spawn interval
			SpawnRandomObject(playerPos, windowWidth, windowHeight);
		}

		// Update animation frames for each object
		for (auto& object : objects) {
			Coin* coin = dynamic_cast<Coin*>(object);
			HealthBottle* healthBottle = dynamic_cast<HealthBottle*>(object);
			ShieldBottle* shieldBottle = dynamic_cast<ShieldBottle*>(object);

			if (coin) {
				coin->updateTime -= deltaTime; // Reduce the update time by delta time
				if (coin->updateTime <= 0.0f) {
					coin->currentFrame++;
					if (coin->currentFrame >= coin->frameCount) {
						coin->currentFrame = 0;
					}
					coin->updateTime = 1.0f / 8.0f; // Reset update time for the next frame
				}
			}
			else if (healthBottle) {
				healthBottle->updateTime -= deltaTime; // Reduce the update time by delta time
				if (healthBottle->updateTime <= 0.0f) {
					healthBottle->currentFrame++;
					if (healthBottle->currentFrame >= healthBottle->frameCount) {
						healthBottle->currentFrame = 0;
					}
					healthBottle->updateTime = 1.0f / 8.0f; // Reset update time for the next frame
				}
			}
			else if (shieldBottle) {
				shieldBottle->updateTime -= deltaTime; // Reduce the update time by delta time
				if (shieldBottle->updateTime <= 0.0f) {
					shieldBottle->currentFrame++;
					if (shieldBottle->currentFrame >= shieldBottle->frameCount) {
						shieldBottle->currentFrame = 0;
					}
					shieldBottle->updateTime = 1.0f / 8.0f; // Reset update time for the next frame
				}
			}
		}

		// Check for collisions with the knight and remove collided objects
		CheckCollisionsWithKnight(playerPos);
	}



	void CheckCollisionsWithKnight(const Vector2& playerPos) {
		Rectangle knightCollisionRec = knight.getCollisionRec();

		for (auto it = objects.begin(); it != objects.end(); /* Increment conditionally inside loop */) {
			GameObject* object = *it;

			float distance = Vector2Distance(object->worldPos, playerPos);
			if (distance < object->radius + knightCollisionRec.width / 2.0f) {
				// Collision detected between object and knight, handle effects based on object type
				Coin* coin = dynamic_cast<Coin*>(object);
				HealthBottle* healthBottle = dynamic_cast<HealthBottle*>(object);
				ShieldBottle* shieldBottle = dynamic_cast<ShieldBottle*>(object);

				if (coin) {
					// Coin: Gain 50 points
					pointSystem.increaseScore(50); // Increment the score by 50
					soundManager.PlayCollectSound();
				}
				else if (healthBottle) {
					// Health Bottle: Gain 10 health
					if (knight.getHealth() + 10.0f > 100.0f) {
						// Ensure health won't exceed max (100)
						knight.takeDamage(100.0f - knight.getHealth());
					}
					else {
						knight.takeDamage(-10); // Increase health
					}
					soundManager.PlayCollectSound();
				}
				else if (shieldBottle) {
					// Shield Bottle: Gain 20 shield
					if (knight.getShield() + 20.0f > 100.0f) {
						// Ensure shield won't exceed max (100)
						knight.increaseShield(100.0f - knight.getShield());
					}
					else {
						knight.increaseShield(20); // Increase shield by 20 when shield bottle is collected
					}
					soundManager.PlayCollectSound();
				}

				// Remove the object after interaction
				delete object;
				it = objects.erase(it);
			}
			else {
				++it;
			}
		}
	}





	void SpawnRandomObject(const Vector2& playerPos, int windowWidth, int windowHeight) {
		float objectRadius = 10.0f;
		Vector2 position;

		// Get scaled map dimensions from the Map instance
		float mapWidth = gameMap.GetWidth() * gameMap.GetScale();
		float mapHeight = gameMap.GetHeight() * gameMap.GetScale();

		// Calculate the map boundaries based on player position and window size
		float minX = std::max(playerPos.x - windowWidth / 2.0f, 0.0f);
		float minY = std::max(playerPos.y - windowHeight / 2.0f, 0.0f);
		float maxX = std::min(playerPos.x + windowWidth / 2.0f, mapWidth);
		float maxY = std::min(playerPos.y + windowHeight / 2.0f, mapHeight);

		// Randomly spawn objects within the expanded map boundaries
		position = {
			static_cast<float>(GetRandomValue(static_cast<int>(minX), static_cast<int>(maxX))),
			static_cast<float>(GetRandomValue(static_cast<int>(minY), static_cast<int>(maxY)))
		};
		// Randomly determine the object type based on different spawn rates
		int objectType = GetRandomObjectType();

		switch (objectType) {
		case 0:
			objects.push_back(new Coin(position));
			break;
		case 1:
			objects.push_back(new HealthBottle(position));
			break;
		case 2:
			objects.push_back(new ShieldBottle(position));
			break;
		}
	}

	int GetRandomObjectType() {
		// Generate a random number between 1 and 100
		int randomValue = GetRandomValue(1, 100);

		// Adjust the ranges based on desired spawn rates
		if (randomValue <= 50) {
			// 50% chance for Coin
			return 0; // Coin
		}
		else if (randomValue <= 80) {
			// 30% chance for HealthBottle
			return 1; // HealthBottle
		}
		else {
			// 20% chance for ShieldBottle
			return 2; // ShieldBottle
		}
	}


	bool IsPositionOccupied(Vector2 position, float objectRadius) {
		// Check if the position is occupied by other objects
		for (const auto& object : objects) {
			float distance = Vector2Distance(object->worldPos, position);
			if (distance < object->radius + objectRadius) {
				return true; // Position is occupied
			}
		}
		return false; // Position is available
	}

	void DrawObjects(const Vector2& mapOffset) const {
		for (const auto& object : objects) {
			// Calculate screen position based on map offset
			Vector2 screenPos = Vector2Subtract(object->worldPos, mapOffset);
			object->Draw(screenPos);
		}
	}
private:
	Map& gameMap;
	Character& knight;
	PointSystem& pointSystem;
	SoundManager& soundManager;

};
