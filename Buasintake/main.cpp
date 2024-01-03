#include "Game.h"

int main() {
	const int windowWidth{ 800 };
	const int windowHeight{ 800 };

	GameManager gameManager(windowWidth, windowHeight);
	gameManager.RunGame();

	return 0;
}