#include "Map.h" 
#include "raymath.h"


Map::Map(const char* filePath, float scale) {
	mapTexture = LoadTexture(filePath); // Loads the map texture from the provided file path
	mapScale = scale; // Sets the scale of the map
}

// Draws the map texture based on the player's world position
void Map::Draw(const Vector2& playerWorldPos) {
	Vector2 mapPos = Vector2Scale(playerWorldPos, -1.f); // Calculates the map position based on player's world position
	DrawTextureEx(mapTexture, mapPos, 0.0, mapScale, WHITE); // Draws the map texture on the screen
}

// Checks if the player is colliding with the map bounds
bool Map::IsCollidingWithMapBounds(const Rectangle& playerCollisionRec, const Vector2& playerWorldPos, int windowWidth, int windowHeight) const {
	return (playerWorldPos.x < -10.f || // Checks if the player is beyond the left map boundary
		playerWorldPos.y < -10.f || // Checks if the player is beyond the top map boundary
		playerWorldPos.x + windowWidth > mapTexture.width * mapScale + 10.f || // Checks if the player is beyond the right map boundary
		playerWorldPos.y + windowHeight > mapTexture.height * mapScale + 10.f); // Checks if the player is beyond the bottom map boundary
}
