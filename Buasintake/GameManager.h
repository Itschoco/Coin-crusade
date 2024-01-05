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
	START_MENU,  // Initial state for the game start menu
	GAMEPLAY,    // State during active gameplay
	GAME_OVER,   // State when the game ends
	EXIT         // State to exit the game
};

class GameManager {
public:
	GameManager(int windowWidth, int windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight) {}

	void RunGame() {
		// Initialization of game elements
		InitWindow(windowWidth, windowHeight, "Coin Crusade"); // Initialize window
		InitAudioDevice(); // Initialize audio device

		// Initialize game objects
		Character knight{ windowWidth, windowHeight };
		SoundManager soundManager;
		Map gameMap("world/map.png", 8.0f);
		PointSystem pointSystem;
		ObjectManager spawner(gameMap, knight, pointSystem, soundManager);
		std::vector<Enemy*> enemies;
		std::vector<Prop> props{
			Prop{Vector2{900.f, 550.f}, LoadTexture("nature_tileset/Rock.png")},
			Prop{Vector2{890.f, 1200.f}, LoadTexture("nature_tileset/Log.png")},
			Prop{Vector2{1270.f, 800.f}, LoadTexture("nature_tileset/Bush.png")},
			Prop{Vector2{1830.f, 500.f}, LoadTexture("nature_tileset/Sign.png")},
			Prop{Vector2{1300.f, 550.f}, LoadTexture("nature_tileset/Rock.png")},
			Prop{Vector2{1900.f, 1800.f}, LoadTexture("nature_tileset/Log.png")},
			Prop{Vector2{1940.f, 450.f}, LoadTexture("nature_tileset/Bush.png")},
			Prop{Vector2{1830.f, 1749.f}, LoadTexture("nature_tileset/Sign.png")}
		};


		bool isDeadSoundPlayed = false; // Flag to track if the dead sound has been played
		
		// Load the background image
		LoadBackgroundImage();

		soundManager.LoadSounds();
		SetTargetFPS(60);
		while (!WindowShouldClose()) {

			BeginDrawing();
			ClearBackground(WHITE);

			// Game state switching and rendering
			switch (currentState) {
			case GameState::START_MENU: // Drawing the start menu
				DrawStartMenu();
				break;

			case GameState::GAMEPLAY: // Drawing the active gameplay
				DrawGame(knight, gameMap, enemies, props, spawner, soundManager, pointSystem, isDeadSoundPlayed);
				break;

			case GameState::GAME_OVER: // Drawing the game over screen
				DrawGameOver(knight, enemies, pointSystem, isDeadSoundPlayed);
				break;

			case GameState::EXIT: // Exiting the game
				CloseWindow();
				soundManager.UnloadSounds();
				return;

			default:
				break;
			}

			EndDrawing();
		}

		CloseWindow(); // Close the game window
		soundManager.UnloadSounds(); // Unload game sounds
	}

	void ResetGame(Character& knight, std::vector<Enemy*>& enemies, PointSystem& pointSystem, bool& isDeadSoundPlayed) {
		// Reset character 
		knight.Reset();

		// Clear existing enemies and reset any other enemy-related properties
		for (auto enemy : enemies) {
			delete enemy;
		}
		enemies.clear();

		// Reset point system, sounds, flags
		pointSystem.resetScore();
		isDeadSoundPlayed = false;

		// Set the game state back to the start menu
		currentState = GameState::START_MENU;
	}

private:
	// Define the current state of the game
	GameState currentState = GameState::START_MENU;
	// Store window dimensions
	int windowWidth, windowHeight;
	Texture2D backgroundImage;

	void LoadBackgroundImage() {
		backgroundImage = LoadTexture("start_menu/start.png");
	}


	// Draw the start menu
	void DrawStartMenu() {
		// Draw the background image first
		DrawTexture(backgroundImage, 0, 0, WHITE);

		// Draw the game title
		DrawText("Coin Crusade", windowWidth / 2 - MeasureText("Coin Crusade", 40) / 2, windowHeight / 4, 40, RED);

		// Draw the start and quit options
		DrawText("Press Enter to Start", windowWidth / 2 - MeasureText("Press Enter to Start", 30) / 2, windowHeight / 2 - 50, 30, RED);
		DrawText("Press Esc to Quit", windowWidth / 2 - MeasureText("Press Esc to Quit", 30) / 2, windowHeight / 2 + 50, 30, RED);

		// Check for key presses to change the game state
		if (IsKeyPressed(KEY_ENTER)) {
			currentState = GameState::GAMEPLAY;
		}
		else if (IsKeyPressed(KEY_ESCAPE)) {
			currentState = GameState::EXIT;
		}
	}

	// Draw the game elements during gameplay
	void DrawGame(Character& knight, Map& gameMap, std::vector<Enemy*>& enemies, std::vector<Prop>& props,
		ObjectManager& spawner, SoundManager& soundManager, PointSystem& pointSystem, bool& isDeadSoundPlayed) {
		// Draw the map based on player position
		gameMap.Draw(knight.getWorldPos());

		// Set targets for enemies
		for (auto enemy : enemies) {
			enemy->setTarget(&knight);
		}

		// Calculate the map offset based on the knight's position
		Vector2 mapOffset = Vector2Subtract(knight.getWorldPos(), Vector2{ windowWidth / 2.0f, windowHeight / 2.0f });

		// Update and draw spawned objects
		spawner.Update(GetFrameTime(), knight.getWorldPos(), windowWidth, windowHeight);
		spawner.DrawObjects(mapOffset);

		// Update the knight's state
		knight.tick(GetFrameTime());

		// Render props
		for (auto& prop : props) {
			prop.Render(knight.getWorldPos());
		}

		// Check if the knight is dead
		if (!knight.getAlive()) {
			if (!isDeadSoundPlayed) {
				soundManager.PlayPlayerDeadSound();
				isDeadSoundPlayed = true;

				// Remove all existing enemies when the player is dead
				for (auto enemy : enemies) {
					delete enemy;
				}
				enemies.clear();
			}

			// Display game over text and handle game over logic
			DrawGameOver(knight, enemies, pointSystem, isDeadSoundPlayed);
		}
		else { // Character is alive
			isDeadSoundPlayed = false;

			// Play background music
			soundManager.PlayBackgroundMusic();

			// Check if it's time to spawn new enemies
			double currentTime = GetTime();
			static double lastSpawnTime = currentTime;

			if (currentTime - lastSpawnTime >= 5.0) {
				Enemy::UpdateEnemySpawn(enemies, &knight, gameMap, windowWidth, windowHeight);
				lastSpawnTime = currentTime;
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

		// Attack enemies on left mouse button click
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			knight.attackEnemies(enemies, soundManager, pointSystem);
		}

		// Draw the player's score
		DrawText(std::to_string(pointSystem.getScore()).c_str(), 10, 50, 30, RED);

		// Update and draw enemies
		for (auto& enemy : enemies) {
			enemy->tick(GetFrameTime());
		}
	}

	// Draw the game over screen
	void DrawGameOver(Character& knight, std::vector<Enemy*>& enemies, PointSystem& pointSystem, bool& isDeadSoundPlayed) {
		// Display game over text
		DrawText("Game Over!", windowWidth / 2 - MeasureText("Game Over!", 40) / 2, windowHeight / 2 - 20, 40, RED);

		// Prompt to restart the game
		DrawText("Press R to Restart", windowWidth / 2 - MeasureText("Press R to Restart", 30) / 2, windowHeight / 2 + 50, 30, RED);

		// Check for key press to reset the game
		if (IsKeyPressed(KEY_R)) {
			ResetGame(knight, enemies, pointSystem, isDeadSoundPlayed);
		}
	}


};
