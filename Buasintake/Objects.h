#ifndef OBJECT_H
#define OBJECT_H

#include "raylib.h"
#include "GameObject.h"
#include "raymath.h"


class Coin : public GameObject {
public:
	Coin(Vector2 pos) : GameObject(pos, 10.0f, "objects/coin.png", 5) {}
};

class HealthBottle : public GameObject {
public:
	HealthBottle(Vector2 pos) : GameObject(pos, 10.0f, "objects/health.png", 4) {}
};

class ShieldBottle : public GameObject {
public:
	ShieldBottle(Vector2 pos) : GameObject(pos, 10.0f, "objects/shield.png", 4) {}
};

#endif // !OBJECT_H
