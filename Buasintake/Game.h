#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include "Soundmanager.h"
#include "Map.h"
#include "PointSystem.h"
#include "ObjectManager.h"
#include <string>
#include <vector>

enum class GameState {
	START_MENU,
	GAMEPLAY,
	GAME_OVER,
	EXIT
};

class GameManager {
public:
	GameManager(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {}

	void RunGame() {
		InitWindow(windowWidth, windowHeight, "Coin Crusade");
		InitAudioDevice(); // Initialize audio device

		Character knight{ windowWidth, windowHeight };
		SoundManager soundManager;
		Map gameMap("world/map.png", 8.0f); // Create a Map object
		PointSystem pointSystem;
		ObjectManager spawner(gameMap, knight, pointSystem, soundManager);
		std::vector<Enemy*> enemies; // Create an empty vector to hold enemies
		std::vector<Prop> props{
		Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
		Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}
		};

		bool isDeadSoundPlayed = false; // Flag to track if the dead sound has been played

		soundManager.LoadSounds();
		SetTargetFPS(60);
		while (!WindowShouldClose()) {
			BeginDrawing();
			ClearBackground(WHITE);

			switch (currentState) {
			case GameState::START_MENU:
				DrawStartMenu();
				break;

			case GameState::GAMEPLAY:
				DrawGame(knight, gameMap, enemies, props, spawner, soundManager, pointSystem, isDeadSoundPlayed);
				break;

			case GameState::GAME_OVER:
				DrawGameOver();
				break;

			case GameState::EXIT:
				CloseWindow();
				soundManager.UnloadSounds();
				return;

			default:
				break;
			}

			EndDrawing();
		}

		CloseWindow();
		soundManager.UnloadSounds();
	}

private:
	GameState currentState = GameState::START_MENU;
	int windowWidth, windowHeight;

	void DrawStartMenu() {
		DrawText("Press Enter to Start", windowWidth / 2 - MeasureText("Press Enter to Start", 30) / 2, windowHeight / 2 - 50, 30, RED);
		DrawText("Press Esc to Quit", windowWidth / 2 - MeasureText("Press Esc to Quit", 30) / 2, windowHeight / 2 + 50, 30, RED);

		if (IsKeyPressed(KEY_ENTER)) {
			currentState = GameState::GAMEPLAY;
		}
		else if (IsKeyPressed(KEY_ESCAPE)) {
			currentState = GameState::EXIT;
		}
	}
	void DrawGame(Character& knight, Map& gameMap, std::vector<Enemy*>& enemies, std::vector<Prop>& props,
		ObjectManager& spawner, SoundManager& soundManager, PointSystem& pointSystem, bool& isDeadSoundPlayed) {

		// Draw the map based on player position
		gameMap.Draw(knight.getWorldPos());

		for (auto enemy : enemies) {
			enemy->setTarget(&knight);
		}

		// Calculate the map offset based on the knight's position
		Vector2 mapOffset = Vector2Subtract(knight.getWorldPos(), Vector2{ windowWidth / 2.0f, windowHeight / 2.0f });

		spawner.Update(GetFrameTime(), knight.getWorldPos(), windowWidth, windowHeight);
		spawner.DrawObjects(mapOffset);

		knight.tick(GetFrameTime());

		for (auto& prop : props) {
			prop.Render(knight.getWorldPos());
		}

		if (!knight.getAlive()) { // Character is not alive
			if (!isDeadSoundPlayed) {
				soundManager.PlayPlayerDeadSound();
				isDeadSoundPlayed = true; // Update the flag

				// Remove all existing enemies when the player is dead
				for (auto enemy : enemies) {
					delete enemy;
				}
				enemies.clear();
			}

			// Calculate text width and position
			const char* gameOverText = "Game Over!";
			Vector2 textMeasure = MeasureTextEx(GetFontDefault(), gameOverText, 40, 1);
			Vector2 textPosition = { windowWidth / 2.0f - textMeasure.x / 2.0f, windowHeight / 2.0f - textMeasure.y / 2.0f };

			DrawText(gameOverText, textPosition.x, textPosition.y, 40, RED);
		}
		else { // Character is alive
			isDeadSoundPlayed = false; // Reset the flag when the player is alive
			// Play background music

			// Check if it's time to spawn new enemies (every 5 seconds for testing)
			double currentTime = GetTime();
			static double lastSpawnTime = currentTime;

			if (currentTime - lastSpawnTime >= 5.0) // Check if 5 seconds have passed for testing
			{
				Enemy::UpdateEnemySpawn(enemies, &knight, gameMap, windowWidth, windowHeight);
				lastSpawnTime = currentTime; // Update the last spawn time
			}
		}

		// Check map bounds
		if (gameMap.IsCollidingWithMapBounds(knight.getCollisionRec(), knight.getWorldPos(), windowWidth, windowHeight)) {
			knight.undoMovement();
		}

		// Check prop collisions
		for (auto& prop : props) {
			if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec())) {
				knight.undoMovement();
			}
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			knight.attackEnemies(enemies, soundManager, pointSystem);
		}

		// Draw score
		DrawText(std::to_string(pointSystem.getScore()).c_str(), 10, 50, 30, RED);

		for (auto& enemy : enemies) {
			enemy->tick(GetFrameTime());
		}
	}



	void DrawGameOver() {
		DrawText("Game Over!", windowWidth / 2 - MeasureText("Game Over!", 40) / 2, windowHeight / 2 - 20, 40, RED);

		if (IsKeyPressed(KEY_ENTER)) {
			currentState = GameState::START_MENU;
		}
	}
};
