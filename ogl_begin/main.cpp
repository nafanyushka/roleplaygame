#include <windows.h>
#include <gl/gl.h>
#include "Creatures.h"
#include "Environment.h"

#pragma comment(lib, "opengl32.lib")

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

//-------------------------VARIABLES---------------------------//

namespace Map
{
	const int MAP_SIZE = 8;
	const int VISION = 4;
	bool isPressed = false;
	Player player(100, 50, 7, 0, humanic, Coord(0, 0));
	char map[MAP_SIZE][MAP_SIZE];

	//FUN
	float tetha = 0.0f;
}

//-------------------------------------------------------------//

//-------------------------FUNCS---------------------------//

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

int getCoordMap(int coord)
{
	return (coord + Map::MAP_SIZE / 2);
}

int getCoordPlane(int coord)
{
	return (coord - Map::MAP_SIZE / 2);
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
				if (place == '#')
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

void drawInventory()
{
	//inv
	float fromX = 6.0f / 8.0f, fromY = -0.66f, toX = 7.0f / 8.0f, toY = 0.66f;
	float squareSize = (toY - fromY) / 8.0f;
	glPushMatrix();
	for (int i = 0; i < 8; i++)
	{
		glColor3f(0.0f, 1.0f, 1.0f);
		drawSquare(squareSize, fromX, fromY + i * squareSize);
		glTranslatef(0.0f, 0.01f, 0.0f);
	}
	glPopMatrix();
}

void drawCharacteristics()
{
	float size = 0.5f;
	float otstup = 0.1f;
	glColor3f(1.0f, 1.0f, 1.0f);
	drawSquare(size / 2.0f, -1.0f + otstup, 1.0f - size);
	glColor3f(0.0f, 1.0f, 0.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size/4.0f) * 1.0f, 1.0f - size + size / 4.0f); //AGIL
	glColor3f(1.0f, 0.0f, 0.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 2.0f, 1.0f - size + size / 4.0f); //POW
	glColor3f(0.0f, 0.0f, 1.0f);
	drawSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 3.0f, 1.0f - size + size / 4.0f); //INT
}

void drawHud()
{
	drawInventory();
	drawCharacteristics();
}

void move(int& x, int& y)
{
	using Map::isPressed;
	int mapX = getCoordMap(Map::player.getCoord().x), mapY = getCoordMap(Map::player.getCoord().y);
	if (!isPressed)
	{
		char* nextC = &Map::map[mapX][mapY + 1];
		if (GetKeyState(VK_UP) < 0) { if (mapY != Map::MAP_SIZE - 1) { if (*nextC != '#') {
			if (*nextC == '.') {
				y++; isPressed = true;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Coord c(mapX, mapY + 1);
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				door->iterate(Map::player);
				if (!(*door).getStatus()) { y++; isPressed = true; }
			}
		} } }
		nextC = &Map::map[mapX][mapY - 1];
		if (GetKeyState(VK_DOWN) < 0) { if (mapY != 0) { if (*nextC != '#') {
			if (*nextC == '.')
			{
				y--; isPressed = true;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Coord c(mapX, mapY - 1);
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				door->iterate(Map::player);
				if (!(*door).getStatus()) {y--; isPressed = true; }
			}
		} } }
		nextC = &Map::map[mapX - 1][mapY];
		if (GetKeyState(VK_LEFT) < 0) { if (mapX != 0) { if (*nextC != '#') { 
			if (*nextC == '.') {
				x--; isPressed = true;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Coord c(mapX - 1, mapY);
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
				door->iterate(Map::player);
				if (!(*door).getStatus()) { x--; isPressed = true; }
			}
		} } }
		nextC = &Map::map[mapX + 1][mapY];
		if (GetKeyState(VK_RIGHT) < 0) { if (mapX != Map::MAP_SIZE - 1) { if(*nextC != '#') {
			if (*nextC == '.') {
				x++; isPressed = true;
			}
			else if (*nextC == '!')
			{
				//*nextC = '.';
				Coord c(mapX + 1, mapY);
				Environment* door = (Environment::getMap()->find(std::pair<int, int>(c.x, c.y))->second);
								door->iterate(Map::player);
if (!(*door).getStatus()) { x++; isPressed = true; }
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
	float d = 2.0f / Map::VISION;
	using Map::map;
	for(int i = 0; i < Map::MAP_SIZE; i++)
		for (int j = 0; j < Map::MAP_SIZE; j++)
		{
			map[i][j] = '.';
		}
	srand(time(NULL));
	//TEST!!!
	//for(int i = 0; i < Map::MAP_SIZE; i++)
	map[4][7] = '#';
	map[4][6] = '#';
	map[4][5] = '#';
	map[4][4] = '!';
	map[4][3] = '#';
	map[4][2] = '#';
	map[4][1] = '#';
	map[4][0] = '#';
	Coord c(4, 4);
	Environment* a = new Door(c, true, 4);
	Environment::getMap()->emplace(std::pair<int, int>(c.x, c.y), a);
	Item item;
	Map::player.pickup(item);
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
		"OpenGL Sample",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024,
		1024,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);

	/* program main loop */
	while (!bQuit)
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
			SwapBuffers(hDC);
			move(playerX, playerY);
			Sleep(1);
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
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