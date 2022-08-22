#ifndef SCENE_ASTEROID_H
#define SCENE_ASTEROID_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAsteroid : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;
	enum BULLET_TYPE
	{
		SINGLE = 0,
		BLACKHOLE,
		BULLET_TYPE_TOTAL,
	};
	enum ENEMY_STATE
	{
		CHASE = 0,
		ATTACK,
		DEATH,
		STATE_TOTAL,
	};
public:
	SceneAsteroid();
	~SceneAsteroid();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	static void Wrap(float& vel, float bound);
	void WrapAsteroids(float& vel, float bound);
	void SpawnEnemy(GameObject *go);

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	void Level(bool);
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_player;
	GameObject* m_background;
	Vector3 m_force;
	int m_objectCount;
	int HP;
	int MaxHP;
	float m_Playerspeed;
	int m_score;
	int m_tempscore;
	double prevElapsed;
	double elapsedTime;
	double bullettime;
	float bounds;
	int wave;
	bool waveclear;
	bool homing;
	bool blackhole;
	int bullettype;
	int bulletdmg;
	int minsize;
	int maxsize;
	bool testmode;
	int tempasteroid;
	int tempblackhole;
	int tempwhitehole;
	bool pause;
	bool godmode;
	bool mainmenu;
	bool instructionmenu;
	bool gamemenu;
	bool upgrademenu;
	bool gameover;
	int selection;
	int hpup;
	int dmgup;
	int bulletspeedup;
	int playerspeedup;
	int healup;
	int passiveheal;
	double passivehealtimer;
	int passivehealingtime;
	int passivehealup;
	bool passivehealing;
	int multiplier;
	int scale;
};

#endif