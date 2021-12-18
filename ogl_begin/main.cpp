#include "main.h"
#pragma comment(lib, "opengl32.lib")

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

void loadTexture(const char*&& path, unsigned int& texture)
{
	int w, h, cnt;
	unsigned char* data = stbi_load(path, &w, &h, &cnt, 0); glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void gameInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadTexture("graphics/human.png", Map::humanTexture);
	loadTexture("graphics/bandit.png", Map::banditTexture);
	loadTexture("graphics/zombie.png", Map::zombieTexture);
	loadTexture("graphics/zoo.png", Map::zooTexture);
	loadTexture("graphics/demon.png", Map::demonicTexture);
	loadTexture("graphics/icy.png", Map::icyTexture);
	loadTexture("graphics/wall.png", Map::wallTexture);
	loadTexture("graphics/chest.png", Map::chestTexture);
	loadTexture("graphics/closedChest.png", Map::closedChestTexture);
	loadTexture("graphics/closedDoor.png", Map::closedDoorTexture);
	loadTexture("graphics/door.png", Map::doorTexture);
	loadTexture("graphics/power.png", Map::powerTexture);
	loadTexture("graphics/intelligence.png", Map::intelligenceTexture);
	loadTexture("graphics/agility.png", Map::agilityTexture);
	loadTexture("graphics/sword.png", Map::swordTexture);
	loadTexture("graphics/potion.png", Map::potionTexture);
	loadTexture("graphics/masterkey.png", Map::masterkeyTexture);
	loadTexture("graphics/bib.png", Map::bibTexture);
	loadTexture("graphics/helmet.png", Map::helmetTexture);
	loadTexture("graphics/boots.png", Map::bootsTexture);
	loadTexture("graphics/pants.png", Map::pantsTexture);
	loadTexture("graphics/inventory.png", Map::inventoryTexture);
	loadTexture("graphics/kalik.png", Map::kalikTexture);
	Map::soundEngine = createIrrKlangDevice();
	Map::player.getPlayerSound() = createIrrKlangDevice();
}

void menuInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	loadTexture("graphics/newbtn.png", Map::newbtn);
	loadTexture("graphics/exitbtn.png", Map::exitbtn);
	loadTexture("graphics/loadbtn.png", Map::loadbtn);
	loadTexture("graphics/menufon.png", Map::menu);
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
	glDisable(GL_TEXTURE_2D);
	float vertex[] = { x,y,0, x + size,y,0, x + size,y + size,0, x,y + size,0 };
	
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void drawTexturedSquare(float size, float x, float y, unsigned int texture)
{
	float vertex[] = { x,y,0, x + size,y,0, x + size,y + size,0, x,y + size,0 };
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glTexCoordPointer(2, GL_FLOAT, 0, Map::textureMap);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawCreature(float size, float x, float y, Creature* creature)
{
	switch (creature->getType())
	{
	case zombie:
		drawTexturedSquare(size, x, y, Map::zombieTexture);
		break;
	case zoo:
		drawTexturedSquare(size, x, y, Map::zooTexture);
		break;
	case icy:
		drawTexturedSquare(size, x, y, Map::icyTexture);
		break;
	case demonic:
		drawTexturedSquare(size, x, y, Map::demonicTexture);
		break;
	default:
		drawTexturedSquare(size, x, y, Map::banditTexture);
		break;
	}
}

void drawPlayer(float size, float x, float y, Creature* creature)
{
	drawTexturedSquare(size, x, y, Map::humanTexture);
}

void drawInfoSquare(float size, float x, float y, float fading)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f * sin(fading /	16.0f) * 0.5f - 0.05f);
	drawSquare(size, x, y);
}

void drawRectangle(float h, float l, float x, float y)
{
	glDisable(GL_TEXTURE_2D);
	float vertex[] = { x - l / 2.0f,y - h / 2.0f,0, x + l / 2.0f,y - h / 2.0f,0, x + l / 2.0f ,y + h / 2.0f,0, x - l / 2.0f,y + h / 2.0f,0 };
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
}

void printText(std::string text, float x, float y, float resizeCoeff)
//От -1 до 1 указывать.
{
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

void drawButton(std::string text, float h, float l, float x, float y)	
{
	drawRectangle(h, l, x, y);
	printText(text, x - l/2.0f, y, 2.0f);
}

void drawMenu()
{
	drawTexturedSquare(2, -1, -1, Map::menu);
	glPushMatrix();
	glTranslatef(-0.2f, -0.2f, 0);
	drawTexturedSquare(0.4f, 0.5f, 0.45f, Map::newbtn);
	drawTexturedSquare(0.4f, 0.5f, 0.0f, Map::loadbtn);
	drawTexturedSquare(0.4f, 0.5f, -0.45f, Map::exitbtn);
	glPopMatrix();
}

Scene tapOnMenu(float x, float y)
{
	if (x > 0.5f && x < 0.9f && y > 0.45f && y < 0.95f)
	{
		//saveDefaultMap();
		loadDefaultMap();
		saveMap();
		Map::soundEngine->play2D("sound/ost.wav", true);
		Map::soundEngine->setSoundVolume(Map::soundEngine->getSoundVolume() * 0.5);
		return game;
	}
	else if (x > 0.5f && x < 0.9f && y > 0.0f && y < 0.4f)
	{
		if (!load())
		{
			Enemy::clearEnemys();
			Environment::clearMap();
			return menu;
		}
		Map::soundEngine->play2D("sound/ost.wav", true);
		Map::soundEngine->setSoundVolume(Map::soundEngine->getSoundVolume() * 0.5);
		return game;

	}
	else if (x > 0.5f && x < 0.9f && y > -0.45f && y < -0.05f)
	{
		return ext;
	}
	return menu;
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
	drawCreature(d, d * (float)i, d * (float)j, &enemy);
}

void drawWall(float& size, float x, float y)
{
	drawTexturedSquare(size, x, y, Map::wallTexture);
}

void drawChest(float& size, float x, float y, bool isClosed)
{
	if (isClosed) drawTexturedSquare(size, x, y, Map::closedChestTexture);
	else drawTexturedSquare(size, x, y, Map::chestTexture);
}

void drawDoor(float& size, float x, float y, bool isClosed, bool isExit)
{
	if  (isExit) drawTexturedSquare(size, x, y, Map::kalikTexture);
	else if (isClosed) drawTexturedSquare(size, x, y, Map::closedDoorTexture);
	else drawTexturedSquare(size, x, y, Map::doorTexture);
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
					drawWall(d, d * (float)i, d * (float)j);
				}
				else if (place == '!')
				{
					drawDoor(d, d * (float)i, d * (float)j, Environment::getMap()->find(std::pair<int, int>(coordX + i, coordY + j))->second->getStatus(), false);
				}
				else if (place == '?')
				{
					drawDoor(d, d * (float)i, d * (float)j, Environment::getMap()->find(std::pair<int, int>(coordX + i, coordY + j))->second->getStatus(), true);

				}
				else if (place == 'c')
				{
					drawChest(d, d * (float)i, d * (float)j, Environment::getMap()->find(std::pair<int, int>(coordX + i, coordY + j))->second->getStatus());
				}
				if (coordX + i == Map::info.x && coordY + j == Map::info.y)
				{
					drawInfoSquare(d, d * (float)i, d * (float)j, Map::fading);
					Map::fading += 0.1f;
				}
			}
		}
	glColor3f(0.0f, 0.0f, 0.0f);
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

void showInfo(std::string&& info)
{
	if (Map::inventoryInfo < 0 && (Map::info.x < 0 || Map::info.y < 0) && Map::equipmentInfo < 0) return;
	printText(info, -0.75, -0.75, 2.0f);
}

void drawPlayersEquipment(float size, float x, float y, Equipment* eq)
{
	switch ((eq)->getEquipmentType())
	{
	case weapon:
		drawTexturedSquare(size, x, y, Map::swordTexture);
		break;
	case helmet:
		drawTexturedSquare(size, x, y, Map::helmetTexture);
		break;
	case bib:
		drawTexturedSquare(size, x, y, Map::bibTexture);
		break;
	case pants:
		drawTexturedSquare(size, x, y, Map::pantsTexture);
		break;
	case boots:
		drawTexturedSquare(size, x, y, Map::bootsTexture);
		break;
	}
}

void drawItem(float size, float x, float y, Item* item)
{
	switch (item->getType())
	{
	case ItemType::masterkey:
		drawTexturedSquare(size, x, y, Map::masterkeyTexture);
		break;
	case ItemType::potion:
		drawTexturedSquare(size, x, y, Map::potionTexture);
		break;
	case ItemType::equipment:
		drawPlayersEquipment(size, x, y, dynamic_cast<Equipment*>(item));
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
		drawTexturedSquare(squareSize, fromX, fromY + i * squareSize, Map::inventoryTexture);
		if (i < Map::player.getItems())
		{
			drawItem(squareSize / 2.0f, fromX + squareSize / 4.0f, fromY + i * squareSize + squareSize / 4.0f, Map::player.getInventory()[i]);
			//glColor3f(1.0f, 1.0f, 1.0f);
			//drawSquare(squareSize / 2.0f, fromX + squareSize / 4.0f, fromY + i * squareSize + squareSize / 4.0f);
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

void tapOnCharacteristics(float x, float y)
{
	float size = 0.5f;
	float otstup = 0.1f;
	if (x > -1.0f + otstup && x < -1.0f + otstup + size / 2.0f && y > 1.0f - size && y < 1.0f - size + size / 2.0f)
	{
		saveMap();
		if (Map::player.getPlayerSound() != nullptr)
			Map::player.getPlayerSound()->play2D("sound\\save.wav", false);
	}
	else if (x > -1.0f + size + (otstup + size / 4.0f) * 1.0f && x < size / 4.0f + -1.0f + size + (otstup + size / 4.0f) * 1.0f
		&& y > 1.0f - size + size / 4.0f && y < 1.0f - size + size / 4.0f + size / 4.0f)
	{
		Map::player.upStats(agil);
	}
	else if (x > -1.0f + size + (otstup + size / 4.0f) * 2.0f && x < -1.0f + size + (otstup + size / 4.0f) * 2.0f + size / 4.0f
		&& y > 1.0f - size + size / 4.0f && y < 1.0f - size + size / 4.0f + size / 4.0f)
	{
		Map::player.upStats(power);
	}
	else if (x > -1.0f + size + (otstup + size / 4.0f) * 3.0f && x < -1.0f + size + (otstup + size / 4.0f) * 3.0f + size / 4.0f
		&& y > 1.0f - size + size / 4.0f && y < 1.0f - size + size / 4.0f + size / 4.0f)
	{
		Map::player.upStats(intel);
	}
}

void drawCharacteristics()
{
	float size = 0.5f;
	float otstup = 0.1f;
	glColor3f(1.0f, 1.0f, 1.0f);
	drawPlayer(size / 2.0f, -1.0f + otstup, 1.0f - size, &Map::player);
	printText(Map::player.getLvlString(), -1.0f + otstup, 1.0f - size, 2.0f);
	
	drawTexturedSquare(size / 4.0f, -1.0f + size + (otstup + size/4.0f) * 1.0f, 1.0f - size + size / 4.0f, Map::agilityTexture); //AGIL
	printText(Map::player.getAgilityString(), -1.0f + size + (otstup + size / 4.0f) * 1.0f, 1.0f - size + size / 4.0f, 4.0f);
	
	drawTexturedSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 2.0f, 1.0f - size + size / 4.0f, Map::powerTexture); //POW
	printText(Map::player.getPowerString(), -1.0f + size + (otstup + size / 4.0f) * 2.0f, 1.0f - size + size / 4.0f, 4.0f);
	
	drawTexturedSquare(size / 4.0f, -1.0f + size + (otstup + size / 4.0f) * 3.0f, 1.0f - size + size / 4.0f, Map::intelligenceTexture); //INT
	printText(Map::player.getIntelligenceString(), -1.0f + size + (otstup + size / 4.0f) * 3.0f, 1.0f - size + size / 4.0f, 4.0f);

	std::string protection = ""; std::string dmg = ""; std::string movepoints = ""; std::string exp = "";
	protection.append("P: ");
	protection.append(Map::player.getProtectionString());
	dmg.append("D: ");
	dmg.append(Map::player.getDmgString());
	movepoints.append("MP: ");
	movepoints.append(Map::player.getMovepointsString());
	exp.append("EXP: ");
	exp.append(Map::player.getExpString());
	
	printText(protection, -1.0f + otstup, 1.0f - size - 0.03f, 2.0f);
	printText(dmg, -1.0f + otstup, 1.0f - size - 0.06f, 2.0f);
	printText(movepoints, -1.0f + otstup, 1.0f - size - 0.09f, 2.0f);
	printText(exp, -1.0f + otstup, 1.0f - size - 0.12f, 2.0f);
}

void drawHealthBar()
{
	glPushMatrix();
	glTranslatef(-1.0f, -1.0f, 0.0f);
	glLineWidth(16);
	glBegin(GL_LINES);
		glColor3f(0.0f, 0.5f, 0.0f);
		glVertex2f(0.0f, 0.01f);
		glVertex2f(1.0f, 0.01f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 0.01f);
		glVertex2f(1.0f * Map::player.getHp() / Map::player.getMaxHp(), 0.01f);
		//glColor3f(0.0f, 0.0f, 1.0f);
		//glVertex2f(0.0f, 0.0f);
		//glVertex2f(1.0f, 0.0f);
	glEnd();
	glLineWidth(1);
	glPopMatrix();
	glPushMatrix();
	glLoadIdentity();
	printText(Map::player.getHpString(), -0.97f, -0.96f, 3.0f);
	glPopMatrix();
}

void drawEquipment()
{
	float size = 0.5f;
	float otstup = 0.1f;
	float startPosDrawingX = -1.0f + otstup, startPosDrawingY = 0.0f;
	float squareSize = 0.15f;
	glPushMatrix();
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		glColor3f(0.1f, 0.1f, 0.1f);
		drawSquare(squareSize, startPosDrawingX, startPosDrawingY - (float)i * squareSize / 1.5f);
		if (Map::player.getEquipment()[i] != nullptr)
		{
			drawPlayersEquipment(squareSize / 2.0f, startPosDrawingX + squareSize / 4.0f, startPosDrawingY - (float)i * squareSize / 1.5f + squareSize / 4.0f, Map::player.getEquipment()[i]);
		}
		glTranslatef(0, -0.1f, 0);
	}
	glPopMatrix();
}

int tapOnEquipment(float x, float y)
{
	float size = 0.5f;
	float otstup = 0.1f;
	float startPosDrawingX = -1.0f + otstup, startPosDrawingY = 0.0f;
	float squareSize = 0.15f;
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		if (x > startPosDrawingX && x < startPosDrawingX + squareSize && y > startPosDrawingY - (float)i * squareSize / 1.5f && y < startPosDrawingY - (float)i * squareSize / 1.5f + squareSize)
		{
			return i;
		}
		y += 0.1f;
	}
	return -1;
}

void drawHud()
{
	drawInventory();
	drawCharacteristics();
	drawHealthBar();
	drawEquipment();
}

void drawInfo()
{
	if (Map::inventoryInfo != -1)
	{
		if (Map::inventoryInfo < Map::player.getItems())
			showInfo(Map::player.getInventory()[Map::inventoryInfo]->getString());
	}
	else if (Map::equipmentInfo != -1)
	{
		if (Map::player.getEquipment()[Map::equipmentInfo] != nullptr)
		{
			showInfo(dynamic_cast<Item*>(Map::player.getEquipment()[Map::equipmentInfo])->getString());
		}
	}
	else
	{
		Enemy* infoEnemy = Enemy::getEnemy(Map::info.x, Map::info.y);
		Environment* infoEnvironment = Environment::getEnvironment(Map::info.x, Map::info.y);
		if (infoEnemy != nullptr)
			showInfo(infoEnemy->getString());
		if (infoEnvironment != nullptr)
			showInfo(infoEnvironment->getString());
	}
}

void drawLoseScreen()
{
	glPushMatrix();
		glLoadIdentity();
		printText("YOU LOSE!", -0.6, 0, 16.0f);
	glPopMatrix();
}

void drawWinScreen()
{
	glPushMatrix();
	glLoadIdentity();
	printText("YOU WIN!", -0.6, 0, 16.0f);
	glPopMatrix();
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
			else if (*nextC == '!' || *nextC == '?')
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
			else if (*nextC == '!' || *nextC == '?')
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
			else if (*nextC == '!' || *nextC == '?')
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
			else if (*nextC == '!' || *nextC == '?')
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

bool botsMove()
{
	//if(Map::enemys[enemy.getCoord().x][enemy.getCoord().y]);
	/*Map::enemys[enemy.getCoord().x][enemy.getCoord().y] = '-';
	if (enemy.getHp() <= 0) return;
	enemy.goToPlayer(Map::player, Map::MAP_SIZE, Map::map, Map::enemys, Map::pathSize);
	Map::enemys[enemy.getCoord().x][enemy.getCoord().y] = 'e';*/


	if (Map::botLogicIterator != Enemy::getEnemys().end())
	{
		using Map::botLogicIterator;
		if ((*botLogicIterator)->getHp() <= 0) 
		{
			botLogicIterator++;
		}
		else if (Map::pathToPlayer == nullptr)
		{
			Map::pathToPlayer = (*botLogicIterator)->goToPlayer(Map::player, Map::MAP_SIZE, Map::map, Map::enemys, Map::pathSize);
			if (Map::pathToPlayer == nullptr) //МОЖЕТ БЫТЬ АТАКУЕТ ЗНАЧИТ
			{
				if ((*botLogicIterator)->getMovepoints() <= 0)
					botLogicIterator++;
				else { Map::pathI = 0; Sleep(50); return true; }
			}
			else Map::pathI = 0;
			return false;
		}
		else
		{
			if (Map::pathI < Map::pathSize)
			{
				Map::enemys[(*botLogicIterator)->getCoord().x][(*botLogicIterator)->getCoord().y] = '-';
				(*botLogicIterator)->move(Map::pathToPlayer[Map::pathI], Map::MAP_SIZE);
				Map::enemys[(*botLogicIterator)->getCoord().x][(*botLogicIterator)->getCoord().y] = 'e';
				Map::pathI++;
				Sleep(50);
				return true;
			}
			else
			{
				if ((*botLogicIterator)->getMovepoints() <= 0) botLogicIterator++;
				Map::pathI = 0;
				Map::pathSize = 0;
				delete[] Map::pathToPlayer;
				Map::pathToPlayer = nullptr;
				return false;
			}
		}
	}
	/*for (auto i = Enemy::getEnemys().begin(); i != Enemy::getEnemys().end(); i++)
	{
		botMove(**i);
		if ((*i)->getMovepoints() > 0)
			isAll = false;
	}*/
	//Если все боты подвигались даем игроку мувпоинты.
	else {
		Map::botLogicIterator = Enemy::getEnemys().begin();
		Map::player.setMovepoints();
	}
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
	bool launch = true;
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
	// 
	// 
	//saveDefaultMap();
	//loadMap();
	//load();
	// 
	// 

	/*map[1][4] = '#';
	map[2][4] = '#';
	map[3][4] = '#';
	map[0][4] = '#';
	map[4][3] = '#';
	map[4][4] = '#';
	map[4][2] = '!';
	map[4][1] = '#';
	map[4][0] = '#';
	map[3][0] = 'c';
	map[2][0] = 'c';
	map[1][0] = 'c';
	map[1][3] = 'c';
	map[0][3] = 'c';
	map[2][3] = 'c';
	map[3][3] = 'c';
	Coord dr(4, 2);
	Coord ch(3, 0);
	Coord ch1(2, 0);
	Coord ch2(1, 0);
	Coord ch3(1, 3);
	Coord ch4(2, 3);
	Coord ch5(3, 3);
	Coord ch6(0, 3);
	Item item(masterkey);
	Potion potion(50, hp);
	EnchantedArtifactWeapon weapon1(10, 4, 0, 0, 0, 10, 2.0f, zombie);
	ArtifactProtection helm(helmet, 5, 0, 3, 1, 10, 0);
	Protection pan(pants, 5);
	Protection bi(bib, 5);
	Protection boo(boots, 5);
	Door* door = new Door(dr, true, 3);
	Chest* chest = new Chest(ch, item, false, 1);
	Chest* chest1 = new Chest(ch1, potion, false, 4);
	Chest* chest2 = new Chest(ch2, weapon1, true, 4);
	Chest* chest3 = new Chest(ch3, helm, true, 3);
	Chest* chest4 = new Chest(ch4, pan, true, 1);
	Chest* chest5 = new Chest(ch5, bi, true, 1);
	Chest* chest6 = new Chest(ch6, boo, true, 1);
	Map::player.setAgility(3);

	Coord z(9, 9);
	Coord z1(9, 0);
	Enemy enemy(100, 33, 90, 5, zombie, z);
	Enemy enemy1(100, 33, 90, 5, zombie, z1);
	enemys[9][0] = 'e';
	enemys[9][9] = 'e';*/


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
		1920,
		1080,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);

	RECT rct;
	GetClientRect(GetDesktopWindow(), &rct);
	SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	SetWindowPos(hwnd, HWND_TOP, rct.left, rct.top, rct.right, rct.bottom, SWP_SHOWWINDOW);

	/* enable OpenGL for the window */
	EnableOpenGL(hwnd, &hDC, &hRC);
	menuInit();
	gameInit();
	/* program main loop */
	while (!bQuit && !Map::isEnd)
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

			//glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, Map::humanTexture);

			if (launch)
			{
				using Map::h;
				using Map::w;
				w = 1920;
				h = 1080;
				glViewport(0, 0, w, h);
				float k = w / (float)h;
				glLoadIdentity();
				glOrtho(-k, k, -1, 1, -1, 1);
				launch = false;
			}

			switch (Map::scene)
			{
			case menu:
				glPushMatrix();
				drawMenu();
				glPopMatrix();
				//-----------------------------------------------------------------------
				SwapBuffers(hDC);
				Sleep(1);
				break;
			case game:
			{
				
				glPushMatrix();
				/*glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
				glTranslatef(-d / 2, -d / 2, 0.0f);
				glScalef(0.5f, 0.5f, 1.0f);
				float x = 0.0f + (float)playerX * d, y = 0.0f + (float)playerY * d;
				glColor3f(1.0f, 1.0f, 1.0f);
				see(playerX, playerY, d);
				drawPlayer(d, 0.0f, 0.0f, &Map::player); //PLAYER
				glColor3f(1.0f, 1.0f, 1.0f);
				//glTexCoordPointer(2, GL_FLOAT, 0, Map::textureMap);
				//drawNazi(0.1f, 0.25f, 0.25f);
				glScalef(2.0f, 2.0f, 1.0f);
				/*glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
				glPopMatrix();
				drawHud();
				drawInfo();
				//-----------------------------------------------------------------------
				SwapBuffers(hDC);
				//-----------------------------------------------------------------------LOGIC

				if (Map::player.getMovepoints() > 0)
				{
					move(playerX, playerY);
				}
				else
				{
					while (!botsMove());
				}
				//-----------------------------------------------------------------------GRAPHICS
				if (Map::player.getHp() <= 0)
				{
					Map::scene = lose;
					Map::soundEngine->stopAllSounds();
					Map::soundEngine->play2D("sound\\lose.wav", false);
					Map::player.getPlayerSound()->stopAllSounds();
				}
				if (Map::player.getIsEnd()) 
				{
					Map::scene = win;
					Map::soundEngine->stopAllSounds();
					Map::soundEngine->play2D("sound\\win.wav", false);
					Map::player.getPlayerSound()->stopAllSounds();
				}
				Sleep(1);
			}
				break;
			case lose:
			{
				drawLoseScreen();
				Map::animCounter++;
				if (Map::animCounter > 10000) Map::scene = Scene::ext;
				SwapBuffers(hDC);
				Sleep(1);
			}
				break;
			case win:
			{
				drawWinScreen();
				Map::animCounter++;
				if(Map::animCounter > 10000) Map::scene = Scene::ext;
				SwapBuffers(hDC);
				Sleep(1);
			}
				break;
			case ext:
				Map::isEnd = true;
				break;
			}
			//-----------------------------------------------------------------------GRAPHICS
			
		}
	}

	/* shutdown OpenGL */
	DisableOpenGL(hwnd, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hwnd);
	Environment::clearMap();
	Enemy::clearEnemys();
	if(Map::soundEngine != nullptr) Map::soundEngine->drop();
	return 0/*msg.wParam*/;
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
		wCoord = (float)LOWORD(lParam);
		hCoord = -(float)HIWORD(lParam);
		switch (Map::scene)
		{
		case menu:
			wCoord = (wCoord * 2.0f / (float)Map::w - 1.0f) * k + 0.2f;
			hCoord = (hCoord * (2.0f) / (float)Map::h + 1.0f) + 0.2f;
			Map::scene = tapOnMenu(wCoord, hCoord);
			break;
		case game:
			wCoord = (wCoord * 2.0f / (float)Map::w - 1.0f) * k;
			hCoord = (hCoord * (2.0f) / (float)Map::h + 1.0f);
			index = tapOnInventory(wCoord, hCoord);
			if (index != -1)
			{
				Map::player.equip(index);
			}
			else
			{
				tapOnCharacteristics(wCoord, hCoord); //ПРОКАЧКА ХАРАКТЕРИСТИК
			}
			break;
		default:
			break;
		}
		break;
	case WM_RBUTTONDOWN:
		Sleep(10);
		wCoord = ((float)LOWORD(lParam) * 2.0f / (float)Map::w - 1.0f) * k;
		hCoord = (-(float)HIWORD(lParam) * (2.0f) / (float)Map::h + 1.0f);
		index = tapOnInventory(wCoord, hCoord);
		if (index != -1)
		{
			Map::player.dropItem(index);
		}
		else
		{
			index = tapOnEquipment(wCoord, hCoord);
			Map::player.unequip(index);
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
		wCoord = ((float)LOWORD(lParam) * 2.0f / (float)Map::w - 1.0f) * k;
		hCoord = (-(float)HIWORD(lParam) * (2.0f) / (float)Map::h + 1.0f);
		//ПОЛУЧАЕМ ИНДЕКСЫ:
		Map::inventoryInfo = tapOnInventory(wCoord, hCoord); //О КАКОМ ИЗ ИНВЕНТОРЯ ИНФУ
		Map::equipmentInfo = tapOnEquipment(wCoord, hCoord); //О КАКОМ ИЗ ЭКВИПЫ ИНФУ
		wCoord = (wCoord + d / 2) * 2.0f;
		hCoord = (hCoord + d / 2) * 2.0f;
		//Такие преобразования из-за того что мы в мейне скейл и транс делаем с картой чтобы она в два раза меньше отображалась.
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
