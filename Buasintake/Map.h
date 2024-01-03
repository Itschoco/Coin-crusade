#pragma once
#include "raylib.h"

class Map {
public:
	Map(const char* filePath, float scale);
	void Draw(const Vector2& playerWorldPos);
	bool IsCollidingWithMapBounds(const Rectangle& playerCollisionRec, const Vector2& playerWorldPos, int windowWidth, int windowHeight) const;

	// Getter methods for map dimensions and scale
	int GetWidth() const { return mapTexture.width; }
	int GetHeight() const { return mapTexture.height; }
	float GetScale() const { return mapScale; }

private:
	Texture2D mapTexture;
	float mapScale;

};
