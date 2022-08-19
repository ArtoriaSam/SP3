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
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_player = new GameObject(GameObject::GO_SHIP);
	m_player->active = true;
	m_player->scale.Set(1, 1, 1);
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
	elapsedTime += dt;
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
					if (wave > 1 && wave % 10 == 0)
					{
						maxsize++;
					}
					if (wave > 10 && (wave - 5) % 10 == 0)
					{
						maxsize--;
						minsize++;
					}
					if (wave * 5 > 100)
					{
						offset = 100 - (wave * 5);
					}
					waveclear = false;
					wave++;
					for (int i = 0; i < (wave * 5) + offset; ++i)
					{
						//Create
						GameObject* go = FetchGO();
						//if (wave % 10 == 0 && (i + 1) % 10 == 0)
						//{
						//	go->type = GameObject::GO_BLACKHOLE;
						//}
						//else if (wave % 5 == 0 && (i + 1) % 5 == 0)
						//{
						//	go->type = GameObject::GO_WHITEHOLE;
						//}
						//else
						//	go->type = GameObject::GO_ASTEROID;
						int temppos = rand() % 4;
						//int tempscale = 0;
						//if (go->type == GameObject::GO_ASTEROID)
						//	tempscale = rand() % maxsize + minsize;
						//else
						//	tempscale = rand() % 3 + 1;
						if (temppos == 1)
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), 0 + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							//go->vel.Set(Math::RandFloatMinMax(5, 20), Math::RandFloatMinMax(-20, 20), 0);
						}
						else if (temppos == 2)
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(m_worldHeight + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							//go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-5, -20), 0);
						}
						else if (temppos == 3)
						{
							go->pos.Set(Math::RandFloatMinMax(m_worldWidth + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							//go->vel.Set(Math::RandFloatMinMax(-5, -20), Math::RandFloatMinMax(-20, 20), 0);
						}
						else
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), 0 + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							//go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(5, 20), 0);
						}
						//go->scale.Set(tempscale, tempscale, 1);
						//if (go->pos.x < 0.0f && go->vel.x < 0.0f)
						//{
						//	go->vel.x *= -1;
						//}
						//else if (go->pos.x > 0.0f && go->vel.x > 0.0f)
						//{
						//	go->vel.x *= -1;
						//}
						//if (go->pos.y < 0.0f && go->vel.y < 0.0f)
						//{
						//	go->vel.y *= -1;
						//}
						//else if (go->pos.y > 0.0f && go->vel.y > 0.0f)
						//{
						//	go->vel.y *= -1;
						//}
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
					//m_ship->mass += 1.0f * dt;
					/*double diff = elapsedTime - prevElapsed;
					if (diff > bullettime)
					{
						for (int i = 0; i < 12; i++)
						{
							GameObject* go = FetchGO();
							go->type = GameObject::GO_BULLET;
							go->scale.Set(0.2f, 0.2f, 1.0f);
							go->pos = m_ship->pos;
							go->vel = m_ship->vel.Normalized() * BULLET_SPEED;
							prevElapsed = elapsedTime;
						}
					}*/
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
					/*m_ship->mass -= 1.0f * dt;
					if (m_ship->mass < 0)
						m_ship->mass = 0.1f;*/
						/*if (homing == false)
							homing = true;
						else if (homing == true)
							homing = false;*/
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
					for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
					{
						GameObject* go = (GameObject*)*it;
						if (go->active)
						{
							go->active = false;
							if (go->type == GameObject::GO_ASTEROID)
								tempasteroid++;
							else if (go->type == GameObject::GO_BLACKHOLE)
								tempblackhole++;
							else if (go->type == GameObject::GO_WHITEHOLE)
								tempwhitehole++;
						}
					}
					while (tempasteroid + tempblackhole + tempwhitehole > 0)
					{
						GameObject* go = FetchGO();
						if (tempasteroid > 0)
						{
							tempasteroid--;
							go->type = GameObject::GO_ASTEROID;
						}
						else if (tempblackhole > 0)
						{
							tempblackhole--;
							go->type = GameObject::GO_BLACKHOLE;
						}
						else
						{
							tempwhitehole--;
							go->type = GameObject::GO_WHITEHOLE;
						}
						int temppos = rand() % 4;
						int tempscale = 0;
						if (go->type == GameObject::GO_ASTEROID)
							tempscale = rand() % maxsize + minsize;
						else
							tempscale = rand() % 3 + 1;

						if (temppos == 1)
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), 0 + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							go->vel.Set(Math::RandFloatMinMax(5, 20), Math::RandFloatMinMax(-20, 20), 0);
						}
						else if (temppos == 2)
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(m_worldHeight + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-5, -20), 0);
						}
						else if (temppos == 3)
						{
							go->pos.Set(Math::RandFloatMinMax(m_worldWidth + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							go->vel.Set(Math::RandFloatMinMax(-5, -20), Math::RandFloatMinMax(-20, 20), 0);
						}
						else
						{
							go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), 0 + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
							go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(5, 20), 0);
						}
						go->scale.Set(tempscale, tempscale, 1);
						if (go->pos.x < 0.0f && go->vel.x < 0.0f)
						{
							go->vel.x *= -1;
						}
						else if (go->pos.x > 0.0f && go->vel.x > 0.0f)
						{
							go->vel.x *= -1;
						}
						if (go->pos.y < 0.0f && go->vel.y < 0.0f)
						{
							go->vel.y *= -1;
						}
						else if (go->pos.y > 0.0f && go->vel.y > 0.0f)
						{
							go->vel.y *= -1;
						}
					}
				}
			}
			/*if (m_score >= 100)
			{
				//triple = true;
				homing = true;
				if (m_score >= 200)
				{
					//multi = true;
					if (m_score >= 250)
					{
						bullettime = 0.05f;
						if (m_score >= 300)
						{
							//homing = true;
						}
					}
				}
				if (m_tempscore >= 100)
				{
					bulletdmg++;
					m_tempscore -= 100;
				}
			}
			else if (m_score < 100)
			{
				homing = false;
				triple = false;
				multi = false;
				bullettime = 0.1f;
				bulletdmg = 1;
			}*/
			//Exercise 14: use a key to spawn a bullet
			if (Application::IsKeyPressed(VK_SPACE))
			{
				/*int tempint = 0;
				for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
				{
					GameObject* go = (GameObject*)*it;
					if (go->active && go->type == GameObject::GO_BULLET)
					{
						tempint++;
					}
				}
				double diff = elapsedTime - prevElapsed;
				if (tempint < 100 && diff > bullettime)
				{
					if (bullettype == SINGLE)
					{
						GameObject* go = FetchGO();
						go->type = GameObject::GO_BULLET;
						go->scale.Set(0.2f, 0.2f, 1.0f);
						go->pos = m_player->pos;
						go->vel = m_player->vel.Normalized() * BULLET_SPEED;
						prevElapsed = elapsedTime;
					}
				}*/
			}
			//Exercise 15: limit the spawn rate of bullets

			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			Vector3 mousePos((x* (m_worldWidth / w)) + (m_player->pos.x - (m_worldWidth / 2)), ((h - y)* (m_worldHeight / h)) + (m_player->pos.y - (m_worldHeight / 2)), 0);

			//Mouse Section
			//static bool bLButtonState = false;
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
				double diff = elapsedTime - prevElapsed;
				if (tempint < 100 && diff > bullettime)
				{
					GameObject* go = FetchGO();
					go->type = GameObject::GO_BULLET;
					go->scale.Set(0.2f, 0.2f, 1.0f);
					go->pos = m_player->pos;
					go->vel = (mousePos - m_player->pos).Normalized() * BULLET_SPEED;
					prevElapsed = elapsedTime;
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
				float angleInRadians = std::atan2(m_player->vel.y, m_player->vel.x);
				float angleInDegrees = (angleInRadians / Math::PI) * 180.0 - 90.0f;
				m_player->angle = angleInDegrees;
			}
			//Exercise 9: wrap ship position if it leaves screen
			//Wrap(m_player->pos.x, m_worldWidth);
			//Wrap(m_player->pos.y, m_worldHeight);
			for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject* go = (GameObject*)*it;
				if (go->active)
				{
					if (go->type != GameObject::GO_BLACKHOLE)
						go->pos += go->vel * dt * m_speed;
					//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
					if (go->type == GameObject::GO_ASTEROID)
					{
						float dis = go->pos.DistanceSquared(m_player->pos);
						float cRad = (m_player->scale.x + go->scale.x) * (m_player->scale.x + go->scale.x);
						if (dis < cRad)
						{
							go->active = false;
							if (godmode == false)
								HP -= 1;
						}

						//Exercise 13: asteroids should wrap around the screen like the ship
						if (go->active)
						{
							if (go->pos.x < -bounds + (m_player->pos.x - (m_worldWidth/ 2)) || go->pos.x > m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.y < -bounds + (m_player->pos.y - (m_worldHeight / 2)) || go->pos.y > m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2)))
							{
								int temppos = rand() % 4;
								if (temppos == 1)
								{
									go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), 0 + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
									go->vel.Set(Math::RandFloatMinMax(5, 20), Math::RandFloatMinMax(-20, 20), 0);
								}
								else if (temppos == 2)
								{
									go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(m_worldHeight + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
									go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(-5, -20), 0);
								}
								else if (temppos == 3)
								{
									go->pos.Set(Math::RandFloatMinMax(m_worldWidth + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
									go->vel.Set(Math::RandFloatMinMax(-5, -20), Math::RandFloatMinMax(-20, 20), 0);
								}
								else
								{
									go->pos.Set(Math::RandFloatMinMax(-bounds + (m_player->pos.x - (m_worldWidth / 2)), m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2))), Math::RandFloatMinMax(-bounds + (m_player->pos.y - (m_worldHeight / 2)), 0 + (m_player->pos.y - (m_worldHeight / 2))), go->pos.z);
									go->vel.Set(Math::RandFloatMinMax(-20, 20), Math::RandFloatMinMax(5, 20), 0);
								}
								if (go->pos.y < 0.0f && go->vel.y < 0.0f)
								{
									go->vel.y *= -1;
								}
								else if (go->pos.y > 0.0f && go->vel.y > 0.0f)
								{
									go->vel.y *= -1;
								}
							}
							//WrapAsteroids(go->pos.x, m_worldWidth + bounds);
							//WrapAsteroids(go->pos.y, m_worldHeight + bounds);

							for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
							{
								GameObject* go2 = (GameObject*)*it2;
								if (go != go2)
								{
									if (go2->type == GameObject::GO_ASTEROID && go2->active)
									{
										float dis = go->pos.DistanceSquared(go2->pos);
										float rad = (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
										if (dis < rad)
										{
											if (go->scale.x > go2->scale.x)
											{
												go->vel += (go2->scale.x / go->scale.x) * go2->vel;
												go->scale.x -= go2->scale.x;
												go->scale.y -= go2->scale.y;
												go2->active = false;
											}
											else if (go2->scale.x > go->scale.x)
											{
												go2->vel += (go->scale.x / go2->scale.x) * go->vel;
												go2->scale.x -= go->scale.x;
												go2->scale.y -= go->scale.y;
												go->active = false;
											}
											else if (go->scale.x == go2->scale.x)
											{
												go->active = false;
												go2->active = false;
											}
										}
									}
								}
							}
						}
					}
					//Exercise 16: unspawn bullets when they leave screen
					if (go->type == GameObject::GO_BULLET)
					{
						if (go->pos.x > m_worldWidth + bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.x < 0 - bounds + (m_player->pos.x - (m_worldWidth / 2)) || go->pos.y > m_worldHeight + bounds + (m_player->pos.y - (m_worldHeight / 2)) || go->pos.y < 0 - bounds + (m_player->pos.y - (m_worldHeight / 2)))
						{
							go->active = false;
							continue;
						}
						//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
						for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
						{
							GameObject* go2 = (GameObject*)*it2;
							if (go2->type == GameObject::GO_ASTEROID && go2->active)
							{
								float dis = go->pos.DistanceSquared(go2->pos);
								float rad = (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
								if (dis < rad)
								{
									go->active = false;

									if (go2->scale.x - bulletdmg >= 1)
									{
										m_score += bulletdmg;
										m_tempscore += bulletdmg;
										go2->scale.x -= bulletdmg;
										go2->scale.y -= bulletdmg;
									}
									else
									{
										m_score += go2->scale.x;
										m_tempscore += go2->scale.x;
										go2->active = false;
									}
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
						if (homing == true)
						{
							float dist = 0.f;
							Vector3 temp = NULL;
							for (std::vector<GameObject*>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
							{
								GameObject* go2 = (GameObject*)*it2;
								if (go2->type == GameObject::GO_ASTEROID && go2->active)
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
									if (go2->type == GameObject::GO_ASTEROID)
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
									else if (go2->type == GameObject::GO_WHITEHOLE)
									{
										go->scale.x *= go2->scale.x;
										go->scale.y *= go2->scale.y;
										go->vel -= go2->vel;
										go2->active = false;
									}
								}
							}
						}
					}
					if (go->type == GameObject::GO_WHITEHOLE)
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
							m_player->vel += (m_player->pos - go->pos) * (go->scale.x / tempdis) * go->scale.x * go->scale.x;
						}
						if (tempdis < tempRad)
						{
							m_player->vel -= go->vel * go->scale.x * go->scale.x;
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
									go2->vel += (go2->pos - go->pos) * (go->scale.x / dis) * go->scale.x * go->scale.x;
								}
								if (dis < rad)
								{
									if (go2->type == GameObject::GO_ASTEROID)
									{
										go2->vel -= go->vel * go->scale.x * go->scale.x;
									}
									else if (go2->type == GameObject::GO_BULLET)
									{
										go2->vel -= go->vel * go->scale.x * go->scale.x;
									}
									/*else if (go2->type == GameObject::GO_BLACKHOLE)
									{
										go2->scale.x *= go->scale.x;
										go2->scale.y *= go->scale.y;
										go2->vel -= go->vel;
										go->active = false;
									}*/
									/*else if (go2->type == GameObject::GO_WHITEHOLE)
									{

									}*/
								}
							}
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
					if (go->type == GameObject::GO_ASTEROID || go->type == GameObject::GO_BLACKHOLE || go->type == GameObject::GO_WHITEHOLE)
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
		RenderMesh(meshList[GEO_PLAYER], false);
		modelStack.PopMatrix();
		//Exercise 17a: render a ship texture or 3D ship model
		 
		//Exercise 17b:	re-orientate the ship with velocity

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
		RenderMesh(meshList[GEO_AXES], false);
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
		RenderTextOnScreen(meshList[GEO_TEXT], "Start", Color(1, 1, 1), 3, (m_worldWidth / 2) - 34, m_worldHeight / 2 - 10);
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