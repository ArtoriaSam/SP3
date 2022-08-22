#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	prevElapsed = elapsedTime = 0.0;
	bullettime = 0.1f;
	bounds = 5.f;
	wave = 0;
	waveclear = true;
	homing = false;
	blackhole = false;
	bullettype = SINGLE;
	testmode = false;
	tempasteroid = 0;
	tempblackhole = 0;
	tempwhitehole = 0;
	pause = false;
	godmode = false;
	mainmenu = true;
	gamemenu = false;
	upgrademenu = false;
	gameover = false;
	selection = 0;
	m_Playerspeed = 1.0f;
	hpup = 0;
	dmgup = 0;
	bulletspeedup = 0;
	playerspeedup = 0;
	healup = 0;
	passivehealup = 0;
	passivehealingtime = 360;
	passivehealtimer = 0.0;
	passiveheal = 1;
	passivehealing = false;
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (int i = 0; i < 100; ++i) {
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	//Exercise 2b: Initialize m_lives and m_score
	HP = 10;
	MaxHP = 10;
	m_score = 0;
	m_tempscore = 0;
	bulletdmg = 1;
	minsize = 1;
	maxsize = 3;
	multiplier = 0;
	scale = 3;
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_player = new GameObject(GameObject::GO_PLAYER);
	m_player->active = true;
	m_player->scale.Set(scale, scale, 1);
	m_player->pos.Set(m_worldWidth / 2, m_worldHeight / 2);
	m_player->vel.Set(0.1f, 0.1f, 0);
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active) {
			continue;
		}
		go->active = true;
		return go;
	}
	int prevSize = m_goList.size();
	for (int i = 0; i < 10; ++i) {
		m_goList.push_back(new GameObject(GameObject::GO_ASTEROID));
	}
	m_goList.at(prevSize)->active = true;
	return m_goList.at(prevSize);
}

void SceneAsteroid::Update(double dt)
{
	SceneBase::Update(dt);
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	int offset = 0;
	if (gamemenu == true)
	{
		if (passivehealing == true)
		{
			if (passivehealtimer <= 0)
			{
				if (HP < MaxHP)
				{
					passivehealtimer = passivehealingtime * dt;
					HP += passiveheal;
				}
			}
			else
				passivehealtimer -= dt;
		}
		static bool bPState = false;
		if (bPState == false && Application::IsKeyPressed('P'))
		{
			bPState = true;
		}
		if (bPState == true && !Application::IsKeyPressed('P'))
		{
			bPState = false;
			if (pause == false)
				pause = true;
			else if (pause == true)
				pause = false;
		}

		static bool bUState = false;
		if (bUState == false && Application::IsKeyPressed('U'))
		{
			bUState = true;
		}
		if (bUState == true && !Application::IsKeyPressed('U'))
		{
			bUState = false;
			gamemenu = false;
			upgrademenu = true;
		}

	    static bool bVState = false;
		if (bVState == false && Application::IsKeyPressed('V'))
		{
			bVState = true;
		}
		if (bVState == true && !Application::IsKeyPressed('V'))
		{
			bVState = false;
			if (testmode == true)
			{
				m_player->active = true;
				HP = 10;
			}
		}
		if (m_player->active && pause == false)
		{
			if (testmode == false)
			{
				if (waveclear == true)
				{
					waveclear = false;
					wave++;
					if (wave > 10)
					{
						multiplier++;
					}
					if (wave == 1 || (wave - 1) % 10 == 0)
					{
						for (int i = 0; i < 5; i++)
						{
							GameObject* go = FetchGO();
							go->type = GameObject::GO_GOBLIN;
							go->speed = 0.5 + (multiplier * 0.05);
							go->hp = 2 + (multiplier * 0.5) ;
							go->points = 20 + (multiplier * 5);
							go->dmg = 1 + (multiplier * 0.5);
							go->atkspeed = 1.5;
							go->range = 1;
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 2 || (wave - 2) % 10 == 0)
					{
						for (int i = 0; i < 6; i++)
						{
							GameObject* go = FetchGO();
							if (i < 4)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_WOLF;
								go->speed = 2 + (multiplier * 0.05);
								go->hp = 4 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}

					}
					else if (wave == 3 || (wave - 3) % 10 == 0)
					{
						for (int i = 0; i < 10; i++)
						{
							GameObject* go = FetchGO();
							if (i < 6)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_WOLF;
								go->speed = 2 + (multiplier * 0.05);
								go->hp = 4 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 4 || (wave - 4) % 10 == 0)
					{
						for (int i = 0; i < 11; i++)
						{
							GameObject* go = FetchGO();
							if (i < 10)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_HARPY;
								go->speed = 1 + (multiplier * 0.05);
								go->hp = 3 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 2;
								go->range = 2;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 5 || (wave - 5) % 10 == 0)
					{
						for (int i = 0; i < 11; i++)
						{
							GameObject* go = FetchGO();
							if (i < 10)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_BEAR;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 10 + (multiplier * 0.5);
								go->points = 50 + (multiplier * 5);
								go->dmg = 5 + (multiplier * 0.5);
								go->atkspeed = 1;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 6 || (wave - 6) % 10 == 0)
					{
						for (int i = 0; i < 20; i++)
						{
							GameObject* go = FetchGO();
							go->type = GameObject::GO_GOBLIN;
							go->speed = 0.5 + (multiplier * 0.05);
							go->hp = 2 + (multiplier * 0.5);
							go->points = 20 + (multiplier * 5);
							go->dmg = 1 + (multiplier * 0.5);
							go->atkspeed = 1.5;
							go->range = 1;
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 7 || (wave - 7) % 10 == 0)
					{
						for (int i = 0; i < 16; i++)
						{
							GameObject* go = FetchGO();
							if (i < 10)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_WOLF;
								go->speed = 2 + (multiplier * 0.05);
								go->hp = 4 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 8 || (wave - 8) % 10 == 0)
					{
						for (int i = 0; i < 23; i++)
						{
							GameObject* go = FetchGO();
							if (i < 15)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_WOLF;
								go->speed = 2 + (multiplier * 0.05);
								go->hp = 4 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 9 || (wave - 9) % 10 == 0)
					{
						for (int i = 0; i < 33; i++)
						{
							GameObject* go = FetchGO();
							if (i < 30)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else
							{
								go->type = GameObject::GO_HARPY;
								go->speed = 1 + (multiplier * 0.05);
								go->hp = 3 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 2;
								go->range = 2;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
					else if (wave == 10 || (wave - 10) % 10 == 0)
					{
						for (int i = 0; i < 29; i++)
						{
							GameObject* go = FetchGO();
							if (i < 20)
							{
								go->type = GameObject::GO_GOBLIN;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 2 + (multiplier * 0.5);
								go->points = 20 + (multiplier * 5);
								go->dmg = 1 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else if (i > 19 && 1 < 24)
							{
								go->type = GameObject::GO_WOLF;
								go->speed = 2 + (multiplier * 0.05);
								go->hp = 4 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 1.5;
								go->range = 1;
							}
							else if (i > 23 && i < 26)
							{
								go->type = GameObject::GO_HARPY;
								go->speed = 1 + (multiplier * 0.05);
								go->hp = 3 + (multiplier * 0.5);
								go->points = 30 + (multiplier * 5);
								go->dmg = 2 + (multiplier * 0.5);
								go->atkspeed = 2;
								go->range = 2;
							}
							else
							{
								go->type = GameObject::GO_BEAR;
								go->speed = 0.5 + (multiplier * 0.05);
								go->hp = 10 + (multiplier * 0.5);
								go->points = 50 + (multiplier * 5);
								go->dmg = 5 + (multiplier * 0.5);
								go->atkspeed = 1;
								go->range = 1;
							}
							go->state = CHASE;
							go->elapsedtime = 0.0f;
							go->scale.Set(scale, scale, 1);
							SpawnEnemy(go);
						}
					}
				}
			}

			if (Application::IsKeyPressed('9'))
			{
				m_speed = Math::Max(0.f, m_speed - 0.1f);
			}
			if (Application::IsKeyPressed('0'))
			{
				m_speed += 0.1f;
			}
			m_force.SetZero();
			//Exercise 6: set m_force values based on WASD
			if (Application::IsKeyPressed('W'))
			{
				if (Application::IsKeyPressed('A'))
				{
					m_player->vel.Set(-10.f, 10.f, 0);
				}
				else if (Application::IsKeyPressed('S'))
				{
					m_player->vel.Set(0, 0, 0);
				}
				else if (Application::IsKeyPressed('D'))
				{
					m_player->vel.Set(10.f, 10.f, 0);
				}
				else
					m_player->vel.Set(0, 10.f, 0);
			}
			else if (Application::IsKeyPressed('A'))
			{
				if (Application::IsKeyPressed('S'))
				{
					m_player->vel.Set(-10.f, -10.f, 0);
				}
				else if (Application::IsKeyPressed('D'))
				{
					m_player->vel.Set(0, 0, 0);
				}
				else
					m_player->vel.Set(-10.f, 0 ,0);
			}
			else if (Application::IsKeyPressed('S'))
			{
				if (Application::IsKeyPressed('D'))
				{
					m_player->vel.Set(10.f, -10.f, 0);
				}
				else
					m_player->vel.Set(0, -10.f, 0);
			}
			else if (Application::IsKeyPressed('D'))
			{
				m_player->vel.Set(10.f, 0, 0);
			}
			if (testmode == true)
			{//Exercise 8: use 2 keys to increase and decrease mass of ship
				static bool bGState = false;
				if (bGState == false && Application::IsKeyPressed('G'))
				{
					bGState = true;
				}
				if (bGState == true && !Application::IsKeyPressed('G'))
				{
					bGState = false;
					if (godmode == false)
						godmode = true;
					else if (godmode == true)
						godmode = false;
				}
				static bool bCState = false;
				if (bCState == false && Application::IsKeyPressed('C'))
				{
					bCState = true;
				}
				if (bCState == true && !Application::IsKeyPressed('C'))
				{
					bCState = false;
					for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
					{
						GameObject* go = (GameObject*)*it;
						if (go->active)
						{
							go->active = false;
						}
					}
				}
				static bool bIState = false;
				if (bIState == false && Application::IsKeyPressed('I'))
				{
					bIState = true;
				}
				if (bIState == true & !Application::IsKeyPressed('I'))
				{
					bIState = false;
					m_score += 100;
					m_tempscore += 100;
				}
				static bool bOState = false;
				if (bOState == false && Application::IsKeyPressed('O'))
				{
					bOState = true;
				}
				if (bOState == true && !Application::IsKeyPressed('O'))
				{
					bOState = false;
					m_score -= 100;
					m_tempscore -= 100;
					if (m_score < 0)
					{
						m_score = 0;
					}
					if (m_tempscore < 0)
					{
						m_tempscore = 0;
					}
				}
				//Exercise 11: use a key to spawn some asteroids
				static bool bRState = false;
				if (bRState == false && Application::IsKeyPressed('R'))
				{
					bRState = true;
				}
				if (bRState == true && !Application::IsKeyPressed('R'))
				{
					bRState = false;
					for (int i = 0; i < 25; ++i)
					{
						//Create
						int tempscale = rand() % 3 + 1;
						GameObject* go = FetchGO();
						go->type = GameObject::GO_ASTEROID;
						go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
						go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
						go->scale.Set(tempscale, tempscale, 1);
					}
				}
				static bool bTState = false;
				if (bTState == false && Application::IsKeyPressed('T'))
				{
					bTState = true;
				}
				if (bTState == true && !Application::IsKeyPressed('T'))
				{
					bTState = false;
					for (int i = 0; i < 25; ++i)
					{
						//Create
						int tempscale = rand() % 3 + 1;
						GameObject* go = FetchGO();
						go->type = GameObject::GO_BLACKHOLE;
						go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
						go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
						go->scale.Set(tempscale, tempscale, 1);
					}
				}
				static bool bYState = false;
				if (bYState == false && Application::IsKeyPressed('Y'))
				{
					bYState = true;
				}
				if (bYState == true && !Application::IsKeyPressed('Y'))
				{
					bYState = false;
					for (int i = 0; i < 25; ++i)
					{
						//Create
						int tempscale = rand() % 3 + 1;
						GameObject* go = FetchGO();
						go->type = GameObject::GO_WHITEHOLE;
						go->pos.Set(Math::RandFloatMinMax(0, m_worldWidth), Math::RandFloatMinMax(0, m_worldHeight), go->pos.z);
						go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-20, 20), 0);
						go->scale.Set(tempscale, tempscale, 1);
					}
				}
				static bool bZState = false;
				if (bZState == false && Application::IsKeyPressed('Z'))
				{
					bZState = true;
				}
				if (bZState == true && !Application::IsKeyPressed('Z'))
				{
					bZState = false;
					//Create
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BLACKHOLE;
					go->pos.Set(m_worldWidth / 2 - 25, m_worldHeight / 2, go->pos.z);
					go->vel.Set(0, 0, 0);
					go->scale.Set(3, 3, 1);
				}
				static bool bXState = false;
				if (bXState == false && Application::IsKeyPressed('X'))
				{
					bXState = true;
				}
				if (bXState == true && !Application::IsKeyPressed('X'))
				{
					bXState = false;
					//Create
					GameObject* go = FetchGO();
					go->type = GameObject::GO_WHITEHOLE;
					go->pos.Set(m_worldWidth / 2 + 25, m_worldHeight / 2, go->pos.z);
					go->vel.Set(0, 0, 0);
					go->scale.Set(3, 3, 1);
				}
			}
			static bool bQState = false;
			if (bQState == false && Application::IsKeyPressed('Q'))
			{
				bQState = true;
			}
			if (bQState == true & !Application::IsKeyPressed('Q'))
			{
				bQState = false;
				if (bullettype == SINGLE)
				{
					if (blackhole == true)
						bullettype = BLACKHOLE;
				}
				else
					bullettype--;
			}
			static bool bEState = false;
			if (bEState == false && Application::IsKeyPressed('E'))
			{
				bEState = true;
			}
			if (bEState == true && !Application::IsKeyPressed('E'))
			{
				bEState = false;
				if (bullettype == BLACKHOLE)
				{
					bullettype = SINGLE;
				}
				else if (blackhole == true)
				{
					bullettype++;
				}
			}
			static bool bFState = false;
			if (bFState == false && Application::IsKeyPressed('F'))
			{
				bFState = true;
			}
			if (bFState == true && !Application::IsKeyPressed('F'))
			{
				bFState = false;
				if (testmode == false)
				{
					testmode = true;
					for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
					{
						GameObject* go = (GameObject*)*it;
						if (go->active)
						{
							go->active = false;
						}
					}
				}
				else if (testmode == true)
				{
					testmode = false;
					godmode = false;
				}
			}
			//Exercise 14: use a key to spawn a bullet
			if (Application::IsKeyPressed(VK_SPACE))
			{
			}
			//Exercise 15: limit the spawn rate of bullets

			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			Vector3 mousePos((x* (m_worldWidth / w)) + (m_player->pos.x - (m_worldWidth / 2)), ((h - y)* (m_worldHeight / h)) + (m_player->pos.y - (m_worldHeight / 2)), 0);

			//Mouse Section
			//static bool bLButtonState = false;
			elapsedTime -= dt;
			if (Application::IsMousePressed(0))
			{
				//bLButtonState = true;
				std::cout << "LBUTTON DOWN" << std::endl;
				int tempint = 0;
				for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject* go = (GameObject*)*it;
					if (go->active && go->type == GameObject::GO_BULLET)
					{
						tempint++;
					}
				}
				if (tempint < 100 && elapsedTime <= 0)
				{
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BULLET;
					go->scale.Set(1.0f, 1.0f, 1.0f);
					go->pos = m_player->pos;
					go->vel = (mousePos - m_player->pos).Normalized() * BULLET_SPEED;
					elapsedTime = 120 * bullettime * dt;
				}
			}
			/*else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
				std::cout << "LBUTTON UP" << std::endl;
			}*/
			static bool bRButtonState = false;
			if (!bRButtonState && Application::IsMousePressed(1))
			{
				bRButtonState = true;
				std::cout << "RBUTTON DOWN" << std::endl;
			}
			else if (bRButtonState && !Application::IsMousePressed(1))
			{
				bRButtonState = false;
				std::cout << "RBUTTON UP" << std::endl;
			}

			//Physics Simulation Section

			//Exercise 7: Update ship's velocity based on m_force
			if (Application::IsKeyPressed('W') || Application::IsKeyPressed('A') || Application::IsKeyPressed('S') || Application::IsKeyPressed('D'))
			{
				//Vector3 acceleration = m_force * (1.0f / m_player->mass);
				//m_player->vel += acceleration * dt * m_speed;
				if (m_player->vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
					m_player->vel.Normalize() *= MAX_SPEED;
				m_player->pos += m_player->vel * dt * m_Playerspeed;
				/*float angleInRadians = std::atan2(m_player->vel.y, m_player->vel.x);
				float angleInDegrees = (angleInRadians / Math::PI) * 180.0 - 90.0f;
				m_player->angle = angleInDegrees;*/
			}
			//Exercise 9: wrap ship position if it leaves screen
			//Wrap(m_player->pos.x, m_worldWidth);
			//Wrap(m_player->pos.y, m_worldHeight);
			for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject* go = (GameObject*)*it;
				if (go->active)
				{
					//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
					if (go->type == GameObject::GO_GOBLIN || go->type == GameObject::GO_WOLF || 
						go->type == GameObject::GO_BEAR || go->type == GameObject::GO_HARPY)
					{
						float dis = go->pos.DistanceSquared(m_player->pos);
						float cRad = (m_player->scale.x + go->scale.x) * (m_player->scale.x + go->scale.x);
						if (dis < cRad)
						{
							if (godmode == false)
							{
								if (go->type == GameObject::GO_HARPY)
									HP -= 1;
								else
									HP -= go->dmg;
							}
						}
						switch (go->state)
						{
						    case CHASE:
							{
								dis = go->pos.DistanceSquared(m_player->pos);
								cRad = (m_player->scale.x + (go->range * go->scale.x)) * (m_player->scale.x + (go->range * go->scale.x));
								if (dis < cRad)
								{
									go->state = ATTACK;
									go->elapsedtime = 0.0f;
								}
								else if (go->hp <= 0)
								{
									go->state = DEATH;
								}
								else
								{
									Vector3 temp;
									temp = (m_player->pos - go->pos);
									go->vel = temp.Normalized() * go->speed;
									go->pos += go->vel * dt * m_speed;
								}
								break;
							}
							case ATTACK:
							{
								go->elapsedtime -= dt;
								dis = go->pos.DistanceSquared(m_player->pos);
								cRad = (m_player->scale.x + (go->range * go->scale.x)) * (m_player->scale.x + (go->range * go->scale.x));
								if (dis > cRad)
								{
									go->state = CHASE;
								}
								else if (go->hp <= 0)
								{
									go->state = DEATH;
								}
								else if (elapsedTime <= 0.0f)
								{
									go->elapsedtime = go->atkspeed * 60 * dt;
									if (go->type == GameObject::GO_HARPY)
									{
										GameObject* temp = FetchGO();
										temp->type = GameObject::GO_FEATHER;
										temp->scale.Set(2.0f, 2.0f, 1.0f);
										temp->pos = go->pos;
										temp->dmg = go->dmg;
										temp->vel = (m_player->pos - go->pos).Normalized() * BULLET_SPEED;
									}
									else
									{
										HP -= go->dmg;
									}
								}
								break;
							}
							case DEATH:
							{
								go->active = false;
								m_score += go->points;
								m_tempscore += go->points;
								break;
							}
							default:
								break;
						}
					}
					//Exercise 16: unspawn bullets when they leave screen
					if (go->type == GameObject::GO_BULLET)
					{
						go->pos += go->vel * dt * m_speed;
						if (go->pos.x > m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.x < 0 - bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.y > m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2)) || go->pos.y < 0 - bounds + (m_player->pos.y - (m_worldHeight / 2)))
						{
							go->active = false;
							continue;
						}
						//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
						for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
						{
							GameObject* go2 = (GameObject*)*it2;
							if (go2->active)
							{
								if (go2->type == GameObject::GO_GOBLIN || go2->type == GameObject::GO_WOLF ||
									go2->type == GameObject::GO_BEAR || go2->type == GameObject::GO_HARPY)
								{
									float dis = go->pos.DistanceSquared(go2->pos);
									float rad = (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
									if (dis < rad)
									{
										go2->hp -= bulletdmg;
										go->active = false;
										if (bullettype == BLACKHOLE)
										{
											GameObject* temp = FetchGO();
											temp->type = GameObject::GO_BLACKHOLE;
											temp->pos.Set(go->pos.x, go->pos.y, go->pos.z);
											temp->vel.Set(0, 0, 0);
											temp->scale.Set(3, 3, 1);
											temp->existtimer = 60 * dt;
										}
									}
								}
							}
						}
						if (homing == true)
						{
							float dist = 0.f;
							Vector3 temp = NULL;
							for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
							{
								GameObject* go2 = (GameObject*)*it2;
								if (go2->active)
								{
									if (go2->type == GameObject::GO_GOBLIN || go2->type == GameObject::GO_WOLF ||
										go2->type == GameObject::GO_BEAR || go2->type == GameObject::GO_HARPY)
									{
										float tempdist = go->pos.DistanceSquared(go2->pos);
										if (dist == 0.f || tempdist < dist)
										{
											dist = tempdist;
											temp = (go2->pos - go->pos);
										}
									}
								}
								if (temp != NULL && go->active)
								{
									go->vel = temp.Normalized() * BULLET_SPEED;
								}
							}
						}
					}
					if (go->type == GameObject::GO_BLACKHOLE)
					{
						if (go->pos.x > m_worldWidth + bounds || go->pos.x < 0 - bounds || go->pos.y > m_worldHeight + bounds || go->pos.y < 0 - bounds)
						{
							go->active = false;
							continue;
						}
						float tempdis = go->pos.DistanceSquared(m_player->pos);
						float tempRad = (m_player->scale.x + go->scale.x) * (m_player->scale.x + go->scale.x);
						if (tempdis < go->scale.x * tempRad * 5)
						{
							m_player->vel += -(m_player->pos - go->pos) * (go->scale.x / tempdis) * go->scale.x;
						}
						if (tempdis < tempRad)
						{
							if (godmode == false)
								HP = 0;
						}

						for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
						{
							GameObject* go2 = (GameObject*)*it2;
							if (go != go2 && go2->active)
							{
								float dis = go->pos.DistanceSquared(go2->pos);
								float rad = (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
								if (dis < go->scale.x * rad * 5)
								{
									go2->vel += -(go2->pos - go->pos) * (go->scale.x / dis) * go->scale.x;
								}
								if (dis < rad)
								{
									if (go2->type == GameObject::GO_GOBLIN || go2->type == GameObject::GO_WOLF ||
										go2->type == GameObject::GO_BEAR || go2->type == GameObject::GO_HARPY)
									{
										go2->active = false;
									}
									else if (go2->type == GameObject::GO_BULLET)
									{
										go2->active = false;
									}
									else if (go2->type == GameObject::GO_BLACKHOLE)
									{
										go->scale.x += go2->scale.x;
										go->scale.y += go2->scale.y;
										go->vel += go2->vel;
										go2->active = false;
									}
								}
							}
						}
					}
					if (go->type == GameObject::GO_FEATHER)
					{
						go->pos += go->vel * dt * m_speed;
						if (go->pos.x > m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.x < 0 - bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.y > m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2)) || go->pos.y < 0 - bounds + (m_player->pos.y - (m_worldHeight / 2)))
						{
							go->active = false;
							continue;
						}
						float dis = go->pos.DistanceSquared(m_player->pos);
						float rad = (go->scale.x + m_player->scale.x) * (go->scale.x + m_player->scale.x);
						if (dis < rad)
						{
							HP -= go->dmg;
							go->active = false;
						}
					}
				}
			}
			bool clear = true;
			if (pause == true || testmode == true)
				clear = false;
			for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject* go = (GameObject*)*it;
				if (go->type == GameObject::GO_BLACKHOLE && go->active)
				{
					go->existtimer -= dt;
					if (go->existtimer <= 0)
						go->active = false;
				}
				if (go->active)
				{
					if (go->type == GameObject::GO_GOBLIN || go->type == GameObject::GO_WOLF || 
						go->type == GameObject::GO_BEAR || go->type == GameObject::GO_HARPY ||
						go->type == GameObject::GO_FEATHER || go->type == GameObject::GO_BLACKHOLE)
					{
						clear = false;
						continue;
					}
				}
			}
			if (clear == true)
				waveclear = true;
			if (HP <= 0)
			{
				m_player->active = false;
				gamemenu = false;
				gameover = true;
			}
		}
    }
	else if (mainmenu == true)
	{
	    static bool bUpState = false;
		if (bUpState == false && Application::IsKeyPressed(VK_UP))
		{
			bUpState = true;
		}
		else if (bUpState == true && !Application::IsKeyPressed(VK_UP))
		{
			bUpState = false;
		}

	    static bool bDownState = false;
		if (bDownState == false && Application::IsKeyPressed(VK_DOWN))
		{
			bDownState = true;
		}
		else if (bDownState == true && !Application::IsKeyPressed(VK_DOWN))
		{
			bDownState = false;
		}

	    static bool bSpaceState = false;
		if (bSpaceState == false && Application::IsKeyPressed(VK_SPACE))
		{
			bSpaceState = true;
		}
		else if (bSpaceState == true && !Application::IsKeyPressed(VK_SPACE))
		{
			bSpaceState = false;
			mainmenu = false;
			if (selection == 0)
			{
				instructionmenu = true;
			}
		}
	}
	else if (instructionmenu == true)
	{
	static bool bUpState = false;
	if (bUpState == false && Application::IsKeyPressed(VK_UP))
	{
		bUpState = true;
	}
	else if (bUpState == true && !Application::IsKeyPressed(VK_UP))
	{
		bUpState = false;
	}

	static bool bDownState = false;
	if (bDownState == false && Application::IsKeyPressed(VK_DOWN))
	{
		bDownState = true;
	}
	else if (bDownState == true && !Application::IsKeyPressed(VK_DOWN))
	{
		bDownState = false;
	}

	static bool bSpaceState = false;
	if (bSpaceState == false && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
	}
	else if (bSpaceState == true && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
		instructionmenu = false;
		if (selection == 0)
		{
			elapsedTime = 0;
			gamemenu = true;
			testmode = false;
			godmode = false;
			HP = MaxHP = 10;
			m_score = 0;
			m_tempscore = 0;
			wave = 0;
			m_objectCount = 0;
			m_speed = 1.0f;
			m_Playerspeed = 1.0f;
			homing = false;
			blackhole = false;
			bullettype = SINGLE;
			bulletdmg = 1;
			bullettime = 0.1f;
			m_tempscore = 0;
			hpup = 0;
			dmgup = 0;
			bulletspeedup = 0;
			playerspeedup = 0;
			healup = 0;
			passivehealup = 0;
			passivehealingtime = 360;
			passivehealtimer = 0.0;
			passiveheal = 1;
			passivehealing = false;
			m_player->active = true;
			m_player->pos.Set((m_worldWidth / 2), (m_worldHeight / 2));
		}
	}
	}
	else if (gameover == true)
	{
	static bool bSpaceState = false;
	if (bSpaceState == false && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
	}
	else if (bSpaceState == true && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
		gameover = false;
		if (selection == 0)
		{
			mainmenu = true;
		}
	}
	}
	else if (upgrademenu == true)
	{
	static bool bUpState = false;
	if (bUpState == false && (Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W')))
	{
		bUpState = true;
		if (selection > 0)
			selection--;
	}
	else if (bUpState == true && !Application::IsKeyPressed(VK_UP) && !Application::IsKeyPressed('W'))
	{
		bUpState = false;
	}

	static bool bDownState = false;
	if (bDownState == false && (Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S')))
	{
		bDownState = true;
		if (selection < 8)
			selection++;
	}
	else if (bDownState == true && !Application::IsKeyPressed(VK_DOWN) && !Application::IsKeyPressed('S'))
	{
		bDownState = false;
	}

	static bool bSpaceState = false;
	if (bSpaceState == false && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		
		if (selection == 0 && m_tempscore >= 100 + (healup * 20) && healup < 3 && HP < MaxHP)
		{
			m_tempscore -= 100 + (healup * 20);
			HP = MaxHP;
			++healup;
		}
		else if (selection == 1 && m_tempscore >= 200 + (passivehealup * 50) && passivehealup < 5)
		{
			m_tempscore -= 200 + (passivehealup * 50);
			passivehealingtime -= 60;
			++passivehealup;
			passivehealing = true;
		}
		else if (selection == 2 && m_tempscore >= 100 + (hpup * 20))
		{
			m_tempscore -= 100 + (hpup * 20);
			++HP;
			++MaxHP;
			++hpup;
		}
		else if (selection == 3 && m_tempscore >= 100 + (dmgup * 20))
		{
			m_tempscore -= 100 + (dmgup * 20);
			++bulletdmg;
			++dmgup;
		}
		else if (selection == 4 && m_tempscore >= 100 + (bulletspeedup * 20) && bullettime >= 0.02f)
		{
			m_tempscore -= 100 + (bulletspeedup * 20);
			bullettime -= 0.01f;
			++bulletspeedup;
		}
		else if (selection == 5 && m_tempscore >= 50 + (playerspeedup * 10) && m_Playerspeed <= 2.4f)
		{
			m_tempscore -= 50 + (playerspeedup * 10);
			m_Playerspeed += 0.1f;
			++playerspeedup;
		}
		else if (selection == 6 && m_tempscore >= 750 && homing == false)
		{
			homing = true;
			m_tempscore -= 750;
		}
		else if (selection == 7 && m_tempscore >= 1000 && blackhole == false)
		{
			blackhole = true;
			m_tempscore -= 1000;
		}
		else if (selection == 8)
		{
			selection = 0;
			upgrademenu = false;
			gamemenu = true;
		}
	}
	else if (bSpaceState == true && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}

	static bool bLeftState = false;
	if (bLeftState == false && Application::IsKeyPressed(VK_LEFT))
	{
		bLeftState = true;
	}
	else if (bLeftState == true && !Application::IsKeyPressed(VK_LEFT))
	{
		bLeftState = false;
	}

	static bool bRightState = false;
	if (bRightState == false && Application::IsKeyPressed(VK_RIGHT))
	{
		bRightState = true;
	}
	else if (bRightState == true && !Application::IsKeyPressed(VK_RIGHT))
	{
		bRightState = false;
	}

	static bool bUState = false;
	if (bUState == false && Application::IsKeyPressed('U'))
	{
		bUState = true;
	}
	if (bUState == true && !Application::IsKeyPressed('U'))
	{
		bUState = false;
		selection = 0;
		upgrademenu = false;
		gamemenu = true;
	}
    }
}

void SceneAsteroid::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->angle + 90.f, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		//Exercise 17a: render a ship texture or 3D ship model
		 
		//Exercise 17b:	re-orientate the ship with velocity

		break;
	case GameObject::GO_PLAYER:
		//Exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PLAYER], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BULLET], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BLACKHOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BLACKHOLE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WHITEHOLE:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WHITEHOLE], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_GOBLIN:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_GOBLIN], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WOLF:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WOLF], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BEAR:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BEAR], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_HARPY:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_HARPY], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FEATHER:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(180, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FEATHER], false);
		modelStack.PopMatrix();
		break;
	}
}

void SceneAsteroid::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x + (m_player->pos.x - (m_worldWidth / 2)), camera.position.y + (m_player->pos.y - (m_worldHeight / 2)), camera.position.z,
						camera.target.x + (m_player->pos.x - (m_worldWidth / 2)), camera.target.y + (m_player->pos.y - (m_worldHeight / 2)), camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	std::ostringstream ss;
	if (gamemenu == true)
	{
		//RenderMesh(meshList[GEO_AXES], false);
		modelStack.PushMatrix();
		modelStack.Translate(m_player->pos.x, m_player->pos.y, -1);
		modelStack.Scale(m_worldWidth, m_worldHeight, 1);
		RenderMesh(meshList[GEO_BACKGROUND], false);
		modelStack.PopMatrix();
		if (m_player->active)
			RenderGO(m_player);
		int noofobj = 1;
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				RenderGO(go);
				noofobj++;
			}
		}

		//On screen text
		if (testmode == true)
		{
			ss.str("");
			ss << "Testmode";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 15);
		}
		/*ss.precision(3);
		ss << "Speed: " << m_speed;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);*/

		ss.str("");
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

		RenderTextOnScreen(meshList[GEO_TEXT], "Gunman Survival", Color(1, 0.5, 0.5), 3, 0, 0);

		//Exercise 5a: Render m_lives, m_score
		ss.str("");
		ss << "HP: " << HP << "/" << MaxHP;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 0, m_worldHeight / 2 + 6);
		ss.str("");
		ss << "Shop points: " << m_tempscore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);
		ss.str("");
		ss << "Score: " << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 12);
		ss.str("");
		ss << "Bullet type: ";
		if (bullettype == SINGLE)
			ss << "Normal";
		else if (bullettype == BLACKHOLE)
			ss << "Blackhole";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 1), 3, 0, 15);

		ss.str("");
		ss << "Wave: " << wave;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.5, 0.5, 1), 3, 0, 6);

		/*ss.str("");
		ss << "Gameobj no: " << noofobj;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);*/
		//Exercise 5b: Render position, velocity & mass of ship
		/*ss.str("");
		ss << "Asteroid spawn size: (" << minsize << "," << maxsize + minsize - 1 << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);*/
		if (pause == true)
		{
			ss.str("");
			ss << "Pause";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, (m_worldWidth / 2) - 34, m_worldHeight / 2);
		}
		if (godmode == true)
		{
			ss.str("");
			ss << "Godmode";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 1), 3, 0, 18);
		}
	}
	else if (mainmenu == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Gunman Survival", Color(1, 0, 0), 4, (m_worldWidth / 2) - 55, m_worldHeight / 2);
		ss.str("");
		RenderTextOnScreen(meshList[GEO_TEXT], "Press Space", Color(1, 1, 1), 3, (m_worldWidth / 2) - 42.5, m_worldHeight / 2 - 10);
		RenderTextOnScreen(meshList[GEO_TEXT], "For instructions", Color(1, 1, 1), 3, (m_worldWidth / 2) - 50, m_worldHeight / 2 - 13);
	}
	else if (instructionmenu == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Insructions", Color(1, 0, 0), 4, (m_worldWidth / 2) - 65, m_worldHeight / 2);
		RenderTextOnScreen(meshList[GEO_TEXT], "Controls:", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 6);
		RenderTextOnScreen(meshList[GEO_TEXT], "WASD for movement", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 9);
		RenderTextOnScreen(meshList[GEO_TEXT], "Left Mouse to shoot", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 12);
		RenderTextOnScreen(meshList[GEO_TEXT], "U for shop", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 18);
		RenderTextOnScreen(meshList[GEO_TEXT], "WS or Up/Down Arrow keys", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 21);
		RenderTextOnScreen(meshList[GEO_TEXT], "for shop navigation", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 24);
		RenderTextOnScreen(meshList[GEO_TEXT], "Space to select upgrade", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 27);
		RenderTextOnScreen(meshList[GEO_TEXT], "Press Space to start Game", Color(1, 1, 1), 3, (m_worldWidth / 2) - 65, m_worldHeight / 2 - 33);
	}
	else if (gameover == true)
	{
		ss.str("");
		ss << "Gameover";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, (m_worldWidth / 2) - 38, m_worldHeight / 2);
		ss.str("");
		ss << "Final Score:" << m_score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, (m_worldWidth / 2) - 42, (m_worldHeight / 2) - 3);
		for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->active)
			{
				go->active = false;
			}
		}
	}
	else if (upgrademenu == true)
	{
		Color tempcolor[9];
		for (int i = 0; i < 9; ++i)
		{
			if (i != selection)
			{
				tempcolor[i] = Color(0, 1, 1);
			}
			else
			{
				tempcolor[i] = Color(0, 1, 0);
			}
		}
		ss.str("");
		ss << "Upgrades:";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, (m_worldHeight / 2) - 3);
		ss.str("");
		ss << "Points avaliable:" << m_tempscore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.75, 0.75, 0), 3, 0, (m_worldHeight / 2) - 6);


		ss.str("");
		ss << "Heal(" << 100 + (healup * 20) << ") " << 3 - healup << "/" << 3;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[0], 3, 0, (m_worldHeight / 2) - 12);
		ss.str("");
		ss << "Heals all your health";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[0], 2, 0, (m_worldHeight / 2) - 13);
		ss.str("");
		ss << "+Passive healing";
		if (passivehealup == 5)
			ss << "(MAX)";
		else
			ss << "(" << 200 + (passivehealup * 50) << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[1], 3, 0, (m_worldHeight / 2) - 16);
		ss.str("");
		if (passivehealing == true)
			ss << "Auotmatic healing of 1 HP every " << 6 - passivehealup << " sceonds";
		else
			ss << "Adds passive healing";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[1], 2, 0, (m_worldHeight / 2) - 17);
		ss.str("");
		ss << "+Max HP(" << 100 + (hpup * 20) << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[2], 3, 0, (m_worldHeight / 2) - 20);
		ss.str("");
		ss << "Increase your max HP to " << MaxHP + 1;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[2], 2, 0, (m_worldHeight / 2) - 21);
		ss.str("");
		ss << "+Damage(" << 100 + (dmgup * 20) << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[3], 3, 0, (m_worldHeight / 2) - 24);
		ss.str("");
		ss << "Increase your damage to " << bulletdmg + 1;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[3], 2, 0, (m_worldHeight / 2) - 25);
		ss.str("");
		ss << "+Attack speed";
		if (bullettime >= 0.01f && bullettime <= 0.02f)
			ss << "(MAX)";
		else
			ss << "(" << 100 + (bulletspeedup * 20) << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[4], 3, 0, (m_worldHeight / 2) - 28);
		ss.str("");
		if (bullettime >= 0.01f && bullettime <= 0.02f)
			ss << "Attack speed:" << bullettime;
		else
			ss << "Increase your attack speed to" << bullettime - 0.01f;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[4], 2, 0, (m_worldHeight / 2) - 29);
		ss.str("");
		ss << "+Movement speed";
		if (m_Playerspeed >= 2.45f && m_Playerspeed <= 2.6f)
			ss << "(MAX)";
		else
			ss << "(" << 50 + (playerspeedup * 10) << ")";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[5], 3, 0, (m_worldHeight / 2) - 32);
		ss.str("");
		if (m_Playerspeed >= 2.45f && m_Playerspeed <= 2.6f)
			ss << "Movement speed:" << m_Playerspeed;
		else
			ss << "Increase your movement speed to" << m_Playerspeed + 0.1f;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[5], 2, 0, (m_worldHeight / 2) - 33);
		ss.str("");
		ss << "Homing bullets";
		if (homing == true)
			ss << "(MAX)";
		else
			ss << "(750)";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[6], 3, 0, (m_worldHeight / 2) - 36);
		ss.str("");
		ss << "Adds homing to your bullets";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[6], 2, 0, (m_worldHeight / 2) - 37);
		ss.str("");
		ss << "Blachole bullets";
		if (blackhole == true)
			ss << "(MAX)";
		else
			ss << "(1000)";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[7], 3, 0, (m_worldHeight / 2) - 40);
		ss.str("");
		ss << "Add a new bullet type to your gun";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[7], 2, 0, (m_worldHeight / 2) - 41);
		ss.str("");
		ss << "Back to game";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), tempcolor[8], 3, 0, (m_worldHeight / 2) - 44);
	}
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_player)
	{
		delete m_player;
		m_player = NULL;
	}
}

void SceneAsteroid::Wrap(float& vel, float bound)
{
	if (vel < 0)
	{
		vel += bound;
	}
	else if (vel > bound)
	{
		vel -= bound;
	}
}

void SceneAsteroid::WrapAsteroids(float& vel, float bound)
{
	if (vel < -bounds)
	{
		vel += bound;
	}
	else if (vel > bound)
	{
		vel -= bound;
	}
}

void SceneAsteroid::SpawnEnemy(GameObject* go)
{

	int temppos = rand() % 4;
	if (temppos == 1)
	{
		go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), 0 + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
	}
	else if (temppos == 2)
	{
		go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(m_worldHeight + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
	}
	else if (temppos == 3)
	{
		go->pos.Set(Math::RandFloatMinMax(m_worldWidth + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
	}
	else
	{
		go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), 0 + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
	}
}
