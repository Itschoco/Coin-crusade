#include "Map.h"
#include "raymath.h"

Map::Map(const char* filePath, float scale) {
	mapTexture = LoadTexture(filePath);
	mapScale = scale;
}

void Map::Draw(const Vector2& playerWorldPos) {
	Vector2 mapPos = Vector2Scale(playerWorldPos, -1.f);
	DrawTextureEx(mapTexture, mapPos, 0.0, mapScale, WHITE);
}

bool Map::IsCollidingWithMapBounds(const Rectangle& playerCollisionRec, const Vector2& playerWorldPos, int windowWidth, int windowHeight) const {
	return (playerWorldPos.x < -10.f ||
		playerWorldPos.y < -10.f ||
		playerWorldPos.x + windowWidth > mapTexture.width * mapScale + 10.f ||
		playerWorldPos.y + windowHeight > mapTexture.height * mapScale + 10.f);
}

