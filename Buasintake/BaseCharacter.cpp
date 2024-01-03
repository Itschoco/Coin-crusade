#include "BaseCharacter.h"
#include "raylib.h"
#include "raymath.h"

BaseCharacter::BaseCharacter() {
}

// Revert the character's movement to the last frame's position
void BaseCharacter::undoMovement() {
	worldPos = worldPosLastFrame;
}

// Function to handle taking damage by the character
void BaseCharacter::takeDamage(float damage) {
	if (shield > 0.0f) {
		shield -= damage;
		if (shield < 0.0f) shield = 0.0f;
	}
	else {
		health -= damage;
		if (health <= 0.0f) setAlive(false);
	}
}

// Get the collision rectangle for the character
Rectangle BaseCharacter::getCollisionRec() {
	// Calculate hitbox dimensions 
	float smallerWidth = width * 0.8f;
	float smallerHeight = height * 0.8f;

	return Rectangle{
		getScreenPos().x + (width - smallerWidth) / 2.0f,
		getScreenPos().y + (height - smallerHeight) / 2.0f,
		smallerWidth * scale,
		smallerHeight * scale
	};
}


void BaseCharacter::tick(float deltaTime) {
	worldPosLastFrame = worldPos; // Store the previous frame's position

	// Update animation frame
	runningTime += deltaTime;
	if (runningTime >= updateTime) {
		frame++;
		runningTime = 0.f;
		if (frame > maxFrames) frame = 0;
	}

	// Move character based on velocity and update texture accordingly
	if (Vector2Length(velocity) != 0.0) {
		worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
		rightLeft = (velocity.x < 0.f) ? -1.f : 1.f;
		texture = (Vector2Length(velocity) != 0.0) ? run : idle;
	}
	else {
		texture = idle;
	}
	velocity = {};

	// Draw character
	Rectangle source{ frame * width, 0.f, rightLeft * width, height };
	Rectangle dest{ getScreenPos().x, getScreenPos().y, scale * width, 4.0f * height };
	DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);

	// Draw health bar
	DrawRectangle(getScreenPos().x, getScreenPos().y - 10, scale * width * (health / 100.0f), 5, GREEN);
	DrawRectangleLines(getScreenPos().x, getScreenPos().y - 10, scale * width, 5, BLACK);

	// Draw shield bar
	DrawRectangle(getScreenPos().x, getScreenPos().y - 15, scale * width * (shield / 100.0f), 5, BLUE);
	DrawRectangleLines(getScreenPos().x, getScreenPos().y - 15, scale * width, 5, BLACK);
}
