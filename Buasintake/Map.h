#pragma once
#include "raylib.h"

class Map {
public:
	Map(const char* filePath, float scale);

	// Draws the map texture based on the player's world position
	void Draw(const Vector2& playerWorldPos);

	// Checks collision between the player and the map boundaries
	bool IsCollidingWithMapBounds(const Rectangle& playerCollisionRec, const Vector2& playerWorldPos, int windowWidth, int windowHeight) const;

	// Returns the width of the map texture
	int GetWidth() const { return mapTexture.width; }

	// Returns the height of the map texture
	int GetHeight() const { return mapTexture.height; }

	// Returns the scale of the map
	float GetScale() const { return mapScale; }

private:
	Texture2D mapTexture;
	float mapScale;
};
