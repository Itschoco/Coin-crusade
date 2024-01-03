#pragma once
#ifndef POINT_SYSTEM_H
#define POINT_SYSTEM_H


class PointSystem {
public:
	// Constructor initializing score to zero
	PointSystem() : score(0) {}

	// Getters for retrieving current score 
	int getScore() const { return score; }

	// Increase the score by a specified amount of points
	void increaseScore(int points) { score += points; }

	void resetScore() { score = 0; }

private:
	int score;
};

#endif // !POINT_SYSTEM_H
