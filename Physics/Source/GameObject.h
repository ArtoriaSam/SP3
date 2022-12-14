#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_SHIP,
		GO_ASTEROID,
		GO_BULLET,
		GO_BLACKHOLE,
		GO_WHITEHOLE,
		GO_PLAYER,
		GO_GOBLIN,
		GO_WOLF,
		GO_BEAR,
		GO_HARPY,
		GO_FEATHER,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	float angle;
	bool active;
	float mass;
	float existtimer;
	int state;
	float range, speed, atkspeed, elapsedtime;
	int hp, dmg, points;
	bool isblackholebullet;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif