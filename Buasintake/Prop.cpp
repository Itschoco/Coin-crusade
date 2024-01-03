#include "Prop.h" 
#include "raymath.h"

// Constructor for the Prop class
Prop::Prop(Vector2 pos, Texture2D tex) : worldPos(pos), texture(tex)
{}

// Renders the prop on the screen relative to the knight's position
void Prop::Render(Vector2 knightPos)
{
	Vector2 screenPos{ Vector2Subtract(worldPos, knightPos) };
	DrawTextureEx(texture, screenPos, 0.f, scale, WHITE);
}

// Retrieves the collision rectangle of the prop relative to the knight's position
Rectangle Prop::getCollisionRec(Vector2 knightPos)
{
	Vector2 screenPos{ Vector2Subtract(worldPos, knightPos) };
	return Rectangle{
		screenPos.x,
		screenPos.y,
		texture.width * scale,
		texture.height * scale
	};
}
