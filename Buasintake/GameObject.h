#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"
#include "raymath.h"

class GameObject {
public:
	Vector2 worldPos;       // Position of the object in the world
	float radius;           // Radius of the object
	Texture2D texture;
	int frameCount;         // Total frames in the object's animation
	int currentFrame;       // Current frame in the animation 
	float updateTime;       // Time taken to update frames


	GameObject(Vector2 pos, float r, const char* texturePath, int frames)
		: worldPos(pos), radius(r), frameCount(frames), currentFrame(0), updateTime(0.0f) {
		texture = LoadTexture(texturePath); // Load the texture from the provided path
	}

	//function to draw the object at a specified screen position
	virtual void Draw(const Vector2& screenPos) const {
		// Define source and destination rectangles for drawing the texture
		Rectangle sourceRect{
			static_cast<float>(currentFrame * texture.width / frameCount),
			0.0f,
			static_cast<float>(texture.width / frameCount),
			static_cast<float>(texture.height)
		};
		Rectangle destRect{
			screenPos.x - radius,
			screenPos.y - radius,
			2.0f * radius,
			2.0f * radius
		};
		// Draw the texture on screen using defined rectangles and settings
		DrawTexturePro(texture, sourceRect, destRect, Vector2{ radius, radius }, 0.0f, WHITE);
	}

	float deleteTime = 0.0f; // Time since object creation
};

#endif // !GAME_OBJECT_H
