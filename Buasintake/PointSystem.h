#pragma once
#ifndef POINT_SYSTEM_H
#define POINT_SYSTEM_H




class PointSystem {
public:
	PointSystem() : score(0), kills(0) {}

	int getScore() const { return score; }
	int getKills() const { return kills; }

	void increaseScore(int points) { score += points; }
	void increaseKills() { kills++; }

	void resetScore() { score = 0; kills = 0; }

private:
	int score;
	int kills;
};



#endif // !POINT_SYSTEM_H
