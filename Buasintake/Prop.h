#pragma once
#include "raylib.h"

class Prop
{
public:
	Prop(Vector2 pos, Texture2D tex);

	// Render the prop at a specific position relative to the knight's position
	void Render(Vector2 knightPos);

	// Get the collision rectangle of the prop based on the knight's position
	Rectangle getCollisionRec(Vector2 knightPos);

private:
	Texture2D texture{}; // Texture of the prop
	Vector2 worldPos{}; // Position of the prop in the world
	float scale{ 4.f }; // Scale factor for the prop
};
