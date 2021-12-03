#include <windows.h>
#include <gl/gl.h>
#include "Creatures.h"
#include "Environment.h"
#include "Map.h"
#include "library/stb_easy_font.h"
#pragma comment(lib, "opengl32.lib")

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

//-------------------------VARIABLES---------------------------//
namespace Map
{
	Player player(100, 30, 0, 0, humanic, Coord(getCoordPlane(0), getCoordPlane(0)));
	//const int MAP_SIZE = SIZE;
	bool isPressed = false;
	char map[MAP_SIZE][MAP_SIZE];
	int floor[MAP_SIZE][MAP_SIZE];
	char enemys[MAP_SIZE][MAP_SIZE];
	Coord info(-1, -1);
	int w, h;
	int watchingIndex = 0;
	float fading = 0.0f;
	//FUN
	float tetha = 0.0f;
}
//-------------------------------------------------------------//

//-------------------------FUNCS---------------------------//
void printString(float x, float y, char* text, float r, float g, float b)
{
	static char buffer[99999]; // ~500 chars
	int num_quads;

	num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

	glColor3f(r, g, b);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 16, buffer);
	glDrawArrays(GL_QUADS, 0, num_quads * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void gameInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void drawNazi(float size, float x, float y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(Map::tetha++, 0, 0, 1);
	glScalef(size, size, 1);
	glLineWidth(5);
	glBegin(GL_LINE_STRIP);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0, -0.5);
		glVertex2f(0, 0.5);
		glVertex2f(0.5, 0.5);
	glEnd();
	glRotatef(90, 0, 0, 1);
	glBegin(GL_LINE_STRIP);
		glVertex2f(-0.5, -0.5);
		glVertex2f(0, -0.5);
		glVertex2f(0, 0.5);
		glVertex2f(0.5, 0.5);
	glEnd();
	glRotatef(-90, 0, 0, 1);
	glPopMatrix();
}

void drawSquare(float size, float x, float y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(size, 0.0f);
		glVertex2f(0.f, size);
		glVertex2f(0.f, size);
		glVertex2f(size, 0.0f);
		glVertex2f(size, size);
	glEnd();
	glPopMatrix();
}

void drawInfoSquare(float size, float x, float y, float fading)
{
	glColor4f(0, 1.0f, 1.0f, 1.0f * sin(fading / 4.0f) * 0.5f);
	drawSquare(size, x, y);
}

void drawRectangle(float h, float l, float x, float y)
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(l, 0.0f);
	glVertex2f(0.f, h);
	glVertex2f(0.f, h);
	glVertex2f(l, 0.0f);
	glVertex2f(l, h);
	glEnd();
	glPopMatrix();
}

int getCoordMap(int coord)
{
	return (coord + Map::MAP_SIZE / 2);
}

int getCoordPlane(int coord)
{
	return (coord - Map::MAP_SIZE / 2);
}

void drawEnemy(Enemy& enemy, float& d, int i, int j)
{
	drawRectangle(d, d * (float)enemy.getHp() / (float)enemy.getMaxHp(), d * (float)i, d * (float)j);
}

void see(int& x, int& y, float& d)
{
	int coordX = getCoordMap(x);
	int coordY = getCoordMap(y);
	float fromX = -1.0f, toX = 1.0f, fromY = fromX, toY = toX;
	if (getCoordMap(y) - Map::VISION / 2 <= -1)
	{
		fromY = 0 - d * (float)coordY;
	}
	if (getCoordMap(x) - Map::VISION / 2 <= -1)
	{
		fromX = 0 - d * (float)coordX;
	}
	if (getCoordMap(y) + Map::VISION / 2 > Map::MAP_SIZE)
	{
		toY = 0 + d * (float)(Map::MAP_SIZE - coordY);
	}
	if (getCoordMap(x) + Map::VISION / 2 > Map::MAP_SIZE)
	{
		toX = 0 + d * (float)(Map::MAP_SIZE - coordX);
	}

	for (int i = -Map::VISION / 2; i < Map::VISION / 2; i++)
		for (int j = -Map::VISION / 2; j < Map::VISION / 2; j++)
		{
			if (coordX + i < Map::MAP_SIZE && coordX + i >= 0 && coordY + j < Map::MAP_SIZE && coordY + j >= 0)
			{
				
				char place = Map::map[coordX + i][coordY + j];
				float floor = Map::floor[coordX + i][coordY + j];
				char* enemy = &Map::enemys[coordX + i][coordY + j];
				float cf = 0.03f;
				if (*enemy == 'e')
				{
					glColor3f(0.0f, 1.0f, 0.0f);
					Enemy* e = Enemy::getEnemy(coordX + i, coordY + j);
					if (e != nullptr)
					{
						if (e->getHp() <= 0)
							*enemy = '-';
						else
							drawEnemy(*Enemy::getEnemy(coordX + i, coordY + j), d, i, j);
					}
				}
				else if (place == '.')
				{
					glColor3f(floor * cf, floor * cf, floor * cf);
					drawSquare(d, d * (float)i, d * (float)j);
				}
				else if (place == '#')
				{
					glColor3f(0.5f, 0.5f, 0.3f);
					drawSquare(d, d * (float)i, d * (float)j);
				}
				else if (place == '!')
				{
					if (Environment::getMap()->find(std::pair<int, int>(coordX + i, coordY + j))->second->getStatus()) glColor3f(0.3f, 0.5f, 0.5f);
					else glColor3f(0.1f, 0.1f, 0.8f);
					drawSquare(d, d * (float)i, d * (float)j);
				}
				else if (place == 'c')
				{
					if (Environment::getMap()->find(std::pair<int, int>(coordX + i, coordY + j))->second->getStatus()) glColor3f(0.35f, 0.2f, 0.09f);
					else glColor3f(0.74f, 0.42f, 0.19f);
					drawSquare(d, d * (float)i, d * (float)j);
				}
				if (coordX + i == Map::info.x && coordY + j == Map::info.y)
				{
					drawInfoSquare(d, d * (float)i, d * (float)j, Map::fading);
					Map::fading += 0.1f;
				}
			}
		}
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = -1 * Map::VISION / 2; i < Map::VISION / 2 + 1; i++)
	{
		if (getCoordMap(y + i) > -1 && getCoordMap(y + i) < Map::MAP_SIZE + 1) {
			glBegin(GL_LINES);
				glVertex2f(fromX, d * i);
				glVertex2f(toX, d * i);
			glEnd();
		}
		if (getCoordMap(x + i) > -1 && getCoordMap(x + i) < Map::MAP_SIZE + 1) {
			glBegin(GL_LINES);
				glVertex2f(d * i, fromY);
				glVertex2f(d * i, toY);
			glEnd();
		}
	}
}

Coord mapInfo(int&& x, int&& y, float& mouseX, float& mouseY, float& d)
{
	int coordX = getCoordMap(x);
	int coordY = getCoordMap(y);
	float fromX = -1.0f, toX = 1.0f, fromY = fromX, toY = toX;
	if (getCoordMap(y) - Map::VISION / 2 <= -1)
	{
		fromY = 0 - d * (float)coordY;
	}
	if (getCoordMap(x) - Map::VISION / 2 <= -1)
	{
		fromX = 0 - d * (float)coordX;
	}
	if (getCoordMap(y) + Map::VISION / 2 > Map::MAP_SIZE)
	{
		toY = 0 + d * (float)(Map::MAP_SIZE - coordY);
	}
	if (getCoordMap(x) + Map::VISION / 2 > Map::MAP_SIZE)
	{
		toX = 0 + d * (float)(Map::MAP_SIZE - coordX);
	}

	if (!(fromY < mouseY && toY > mouseY && fromX < mouseX && toX > mouseX)) return Coord(-1, -1);

	for (int i = -Map::VISION / 2; i < Map::VISION / 2; i++)
		for (int j = -Map::VISION / 2; j < Map::VISION / 2; j++)
		{
			if (coordX + i < Map::MAP_SIZE && coordX + i >= 0 && coordY + j < Map::MAP_SIZE && coordY + j >= 0)
			{
				if (mouseX > d * (float)i && mouseX < d * (float)i + d && mouseY > d * (float)j && mouseY < d * (float)j + d)
				{
					return Coord(coordX + i, coordY + j);
				}
			}
		}
	return Coord(-1, -1);
}

void printText(std::string text, float x, float y)
//ќт -1 до 1 указывать.
{
	float resizeCoeff = 2.0f;
	float k = (float)Map::w / (float)Map::h;
	float h = (x / k + 1.0f) * (Map::w / 2.0f);
	float w = (1.0f - y) * (Map::h / 2.0f);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, Map::w, Map::h, 0, -1, 1);
	const char* c = (text.c_str());
	char* cString = new char[text.size() + 1];
	int i = 0;
	while (*c)
	{
		cString[i] = *(c++);
		i++;
	}
	cString[i] = '\0';
	glScalef(resizeCoeff, resizeCoeff, 1.0f);
	printString(h / resizeCoeff, w / resizeCoeff, cString, 1.0f, 1.0f, 1.0f);
	delete[] cString;
	glPopMatrix();
}

void showInfo(std::string&& info)
{
	if (Map::info.x < 0 || Map::info.y < 0) return;
	printText(info, -0.75, -0.75);
}

void chooseItemColor(Item* item)
{
	switch (item->getType())
	{
	case ItemType::masterkey:
		glColor3f(0.38f, 0.38f, 0.38f);
		break;
	case ItemType::potion:
		glColor3f(1.0f, 0.32f, 0.85f);
		break;
	case ItemType::equipment:
		switch (dynamic_cast<Equipment*>(item)->getEquipmentType())
		{
		case weapon:
			glColor3f(0.85f, 0.1f, 0.1f);
			break;
		case helmet:
			glColor3f(0.66f, 0.77f, 0.74f);
			break;
		case bib:
			glColor3f(0.44f, 0.57f, 0.54f);
			break;
		case pants:
			glColor3f(0.38f, 0.47f, 0.45f);
			break;
		case boots:
			glColor3f(0.31f, 0.38f, 0.36f);
			break;	
		}
		break;
	}
}

void drawInventory()
{
	//inv
	float fromX = 6.0f / 8.0f, fromY = -0.66f, toX = 7.0f / 8.0f, toY = 0.66f;
	float squareSize = (toY - fromY) / (float) Player::getInventorySize();
	glPushMatrix();
	for (int i = 0; i < Player::getInventorySize(); i++)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		drawSquare(squareSize, fromX, fromY + i * squareSize);
		if (i < Map::player.getItems())
		{
			chooseItemColor(Map::player.getInventory()[i]);
			drawSquare(squareSize / 2.0f, fromX + squareSize / 4.0f, fromY + i * squareSize + squareSize / 4.0f);
		}
		glTranslatef(0.0f, 0.01f, 0.0f);
	}
	glPopMatrix();
}

int tapOnInventory(float x, float y)
{
	float fromX = 6.0f / 8.0f, fromY = -0.66f, toX = 7.0f / 8.0f, toY = 0.66f;
	float squareSize = (toY - fromY) / (float)Player::getInventorySize();
	for (int i = 0; i < Player::getInventorySize(); i++)
	{
		if (x > fromX && x < fromX + squareSize && y > fromY + i * (squareSize + 0.01f) && y < fromY + i * (squareSize + 0.01f) + squareSize)
			return i;
	}
	return -1;
}

void drawCharacteristics()
{
	float size = 0.5f;
	float otstup = 0.1f;
	glColor3f(1.0f, 1.0f, 1.0f);
	drawSquare(size / 2.0f, -1.0f + otstup, 1.0f - size);
	printText(Map::player.getLvlString(), -1.0f + otstup, 1.0f - size);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size/4.0f) * 1.0f, 1.0f - size + size / 4.0f); //AGIL
	printText(Map::player.getAgilityString(), -1.0f + size + (otstup + size / 4.0f) * 1.0f, 1.0f - size + size / 4.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 2.0f, 1.0f - size + size / 4.0f); //POW
	printText(Map::player.getPowerString(), -1.0f + size + (otstup + size / 4.0f) * 2.0f, 1.0f - size + size / 4.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 3.0f, 1.0f - size + size / 4.0f); //INT
	printText(Map::player.getIntelligenceString(), -1.0f + size + (otstup + size / 4.0f) * 3.0f, 1.0f - size + size / 4.0f);

	std::string protection = ""; std::string dmg = "";
	protection.append("P: ");
	protection.append(Map::player.getProtectionString());
	dmg.append("D: ");
	dmg.append(Map::player.getDmgString());
	printText(protection, -1.0f + otstup, 1.0f - size - 0.03f);
	printText(dmg, -1.0f + otstup, 1.0f - size - 0.06f);
}

void drawHealthBar()
{
	glPushMatrix();
	glTranslatef(-1.0f, -1.0f, 0.0f);
	glLineWidth(16);
	glBegin(GL_LINES);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 0.01f);
		glVertex2f(1.0f * Map::player.getHp() / Map::player.getMaxHp(), 0.01f);
		//glColor3f(0.0f, 0.0f, 1.0f);
		//glVertex2f(0.0f, 0.0f);
		//glVertex2f(1.0f, 0.0f);
	glEnd();
	glLineWidth(1);
	glPopMatrix();
}

void drawHud()
{
	drawInventory();
	drawCharacteristics();
	drawHealthBar();
}

void move(int& x, int& y)
{
	using Map::isPressed;
	int mapX = getCoordMap(Map::player.getCoord().x), mapY = getCoordMap(Map::player.getCoord().y);
	if (!isPressed)
	{
		char* nextC = &Map::map[mapX][mapY + 1];
		char* nextE = &Map::enemys[mapX][mapY + 1];
		Coord c(mapX, mapY + 1);
		if (GetKeyState(VK_UP) < 0) { if (mapY != Map::MAP_SIZE - 1) { if (*nextC != '#') {
			isPressed = true;
			if (*nextE == 'e')
			{
				Enemy* e = Enemy::getEnemy(mapX, mapY + 1);
				if (e != nullptr) { Map::player.hit(*e); return; }
			}
			if (*nextC == '.') {
				Map::player.move(top, Map::MAP_SIZE); return;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				if ((*door).getStatus())
				{
					door->iterate(Map::player);
					if (!(*door).getStatus()) { y++; }
				}
				else { Map::player.move(top, Map::MAP_SIZE); return; }
			}
			else if (*nextC == 'c')
			{
				Environment* chest = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				chest->iterate(Map::player);
			}
		} } }
		nextC = &Map::map[mapX][mapY - 1];
		nextE = &Map::enemys[mapX][mapY - 1];
		c = Coord(mapX, mapY - 1);
		if (GetKeyState(VK_DOWN) < 0) { if (mapY != 0) { if (*nextC != '#') {
			isPressed = true;
			if (*nextE == 'e')
			{
				Enemy* e = Enemy::getEnemy(mapX, mapY - 1);
				if (e != nullptr) { Map::player.hit(*e); return; }
			}
			if (*nextC == '.')
			{
				Map::player.move(bottom, Map::MAP_SIZE); return;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				if ((*door).getStatus())
				{
					door->iterate(Map::player);
					if (!(*door).getStatus()) { y--; }
				}
				else { Map::player.move(bottom, Map::MAP_SIZE); return; }
			}
			else if (*nextC == 'c')
			{
				Environment* chest = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				chest->iterate(Map::player);
			}
		} } }
		nextC = &Map::map[mapX - 1][mapY];
		nextE = &Map::enemys[mapX - 1][mapY];
		c = Coord(mapX - 1, mapY);
		if (GetKeyState(VK_LEFT) < 0) { if (mapX != 0) { if (*nextC != '#') { 
			isPressed = true;
			if (*nextE == 'e')
			{
				Enemy* e = Enemy::getEnemy(mapX - 1, mapY);
				if (e != nullptr) { Map::player.hit(*e); return; }
			}
			if (*nextC == '.') {
				Map::player.move(left, Map::MAP_SIZE); return;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				if ((*door).getStatus())
				{
					door->iterate(Map::player);
					if (!(*door).getStatus()) { x--; }
				}
				else { Map::player.move(left, Map::MAP_SIZE); return; }
			}
			else if (*nextC == 'c')
			{
				Environment* chest = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				chest->iterate(Map::player);
			}
		} } }
		nextC = &Map::map[mapX + 1][mapY];
		nextE = &Map::enemys[mapX + 1][mapY];
		c = Coord(mapX + 1, mapY);
		if (GetKeyState(VK_RIGHT) < 0) { if (mapX != Map::MAP_SIZE - 1) { if(*nextC != '#') {
			isPressed = true;
			if (*nextE == 'e')
			{
				Enemy* e = Enemy::getEnemy(mapX + 1, mapY);
				if (e != nullptr) { Map::player.hit(*e); return; }
			}
			if (*nextC == '.') {
				Map::player.move(right, Map::MAP_SIZE); return;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				if ((*door).getStatus())
				{
					door->iterate(Map::player);
					if (!(*door).getStatus()) { x++; }
				}
				else { Map::player.move(right, Map::MAP_SIZE); return; }
			}
			else if (*nextC == 'c')
			{
				Environment* chest = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				chest->iterate(Map::player);
			}
		} } }
	}
	else
	{
		if (!(GetKeyState(VK_UP) < 0 || GetKeyState(VK_DOWN) < 0 || GetKeyState(VK_LEFT) < 0 || GetKeyState(VK_RIGHT) < 0))
			isPressed = false;
	}
	Map::player.setCoord(Coord(x, y));
}

void botMove(Enemy& enemy)
{
	//if(Map::enemys[enemy.getCoord().x][enemy.getCoord().y]);
	Map::enemys[enemy.getCoord().x][enemy.getCoord().y] = '-';
	if (enemy.getHp() <= 0) return;
	enemy.goToPlayer(Map::player, Map::MAP_SIZE, Map::map, Map::enemys);
	Map::enemys[enemy.getCoord().x][enemy.getCoord().y] = 'e';
}
//---------------------------------------------------------//

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hwnd;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	float theta = 0.0f;
	float scale = 0.0001f;
	srand(time(NULL));
	float d = 2.0f / Map::VISION;
	using Map::map; using Map::floor; using Map::enemys;
	for(int i = 0; i < Map::MAP_SIZE; i++)
		for (int j = 0; j < Map::MAP_SIZE; j++)
		{
			map[i][j] = '.';
			floor[i][j] = rand() % 10;
			enemys[i][j] = '-';
		}
	//TEST!!!
	//for(int i = 0; i < Map::MAP_SIZE; i++)
	map[1][4] = '#';
	map[2][4] = '#';
	map[3][4] = '#';
	map[0][4] = '#';
	map[4][3] = '#';
	map[4][2] = '!';
	map[4][1] = '#';
	map[4][0] = '#';
	map[3][0] = 'c';
	map[2][0] = 'c';
	map[1][0] = 'c';
	Coord dr(4, 2);
	Coord ch(3, 0);
	Coord ch1(2, 0);
	Coord ch2(1, 0);
	Item item(masterkey);
	Potion potion(50, hp);
	EnchantedArtifactWeapon weapon1(70, 4, 0, 0, 20, 10, 2.0f, zombie);
	Door* door = new Door(dr, false, 1);
	Chest* chest = new Chest(ch, item, false, 1);
	Chest* chest1 = new Chest(ch1, potion, false, 4);
	Chest* chest2 = new Chest(ch2, weapon1, true, 4);
	Map::player.setAgility(3);
	//Environment::getMap()->emplace(std::pair<int, int>(ch.x, ch.y), chest);
	//Environment::getMap()->emplace(std::pair<int, int>(ch1.x, ch1.y), chest1);
	//Environment::getMap()->emplace(std::pair<int, int>(ch2.x, ch2.y), chest2);

	Coord z(9, 9);
	Coord z1(9, 0);
	Enemy enemy(100, 33, 90, 5, zombie, z);
	Enemy enemy1(100, 33, 90, 5, zombie, z1);
	enemys[9][0] = 'e';
	enemys[9][9] = 'e';
	//TEST!!!
	/* register window class */
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "GLSample";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


	if (!RegisterClassEx(&wcex))
		return 0;

	/* create main window */
	hwnd = CreateWindowEx(0,
		"GLSample",
		"Dungeon",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1080,
		1080,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	gameInit();
	/* program main loop */
	while (!bQuit && Map::player.getHp() > 0)
	{
		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			/* OpenGL animation code goes here */
			int playerX = Map::player.getCoord().x, playerY = Map::player.getCoord().y;
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			//-----------------------------------------------------------------------GRAPHICS
			glPushMatrix();
				glTranslatef(-d / 2, -d / 2, 0.0f);
				glScalef(0.5f, 0.5f, 1.0f);
				float x = 0.0f + (float)playerX * d, y = 0.0f + (float)playerY * d;
				glColor3f(1.0f, 1.0f, 1.0f);
				see(playerX, playerY, d);
				drawSquare(d, 0.0f, 0.0f); //PLAYER
				//drawNazi(0.1f, 0.25f, 0.25f);
				glScalef(2.0f, 2.0f, 1.0f);
			glPopMatrix();
			drawHud();
			Enemy* infoEnemy = Enemy::getEnemy(Map::info.x, Map::info.y);
			if(infoEnemy != nullptr) 
				showInfo(infoEnemy->getString());
			//-----------------------------------------------------------------------
			SwapBuffers(hDC);
			//-----------------------------------------------------------------------LOGIC
			if (Map::player.getMovepoints() > 0)
			{
				move(playerX, playerY);
			}
			else
			{
				
				Sleep(50);
				bool isAll = false;
				for (auto i = Enemy::getEnemys().begin(); i != Enemy::getEnemys().end(); i++)
				{
					botMove(**i);
					if ((*i)->getMovepoints() <= 0)
						isAll = true;
					else isAll = false;
				}
				//≈сли все боты подвигались даем игроку мувпоинты.
				if(isAll) Map::player.setMovepoints();
			}
			//-----------------------------------------------------------------------GRAPHICS
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);
	Environment::clearMap();
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	float wCoord, hCoord;
	float k = Map::w / (float)Map::h;
	float d = 2.0f / Map::VISION;
	int index;
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Sleep(10);
		wCoord = ((float)LOWORD(lParam) * 2.0f / (float)Map::w - 1.0f) * k;
		hCoord = (-(float)HIWORD(lParam) * (2.0f) / (float)Map::h + 1.0f);
		index = tapOnInventory(wCoord, hCoord);
		if (index != -1)
		{
			Map::player.equip(index);
		}
		break;
	case WM_SIZE:
		using Map::h;
		using Map::w;
		w = LOWORD(lParam);
		h = HIWORD(lParam);
		glViewport(0, 0, w, h);
		k = w / (float)h;
		glLoadIdentity();
		glOrtho(-k, k, -1, 1, -1, 1);
		break;
	case WM_MOUSEMOVE:
		wCoord = (((float)LOWORD(lParam) * (2.0f) / (float)Map::w - 1.0f) * k + d / 2) * 2.0f;
		hCoord = ((-(float)HIWORD(lParam) * (2.0f) / (float)Map::h + 1.0f) + d / 2) * 2.0f;
		//“акие преобразовани€ из-за того что мы в мейне скейл и транс делаем с картой чтобы она в два раза меньше отображалась.
		Map::info = mapInfo(Map::player.getCoord().x, Map::player.getCoord().y, wCoord, hCoord, d);
		break;
	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
	PIXELFORMATDESCRIPTOR pfd;

	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hwnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	iFormat = ChoosePixelFormat(*hDC, &pfd);

	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);

	wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hwnd, hDC);
}