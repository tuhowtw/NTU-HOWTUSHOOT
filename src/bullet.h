#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <string>
using namespace sf;

class Bullet {
private:
	Vector2f position;
	Vector2f shooterXY;
	Vector2f defaultPosition;
	Vector2f targetXY;
	Sprite *shooterSprite;
	RenderWindow *window;
	RectangleShape bulletShape;
	float movingSpeed = 2, speedUnit = 1;
	float xd, yd;
	float rate;
	float xMove, yMove;
	bool broken = false;
	bool active = false;
	int health;

	int xSize;
	int ySize;

	Transform transform;


public:
	Bullet(Sprite *shooterSpriteIn, Vector2f targetXY, RenderWindow *inWin, std::string color, int sizeX, int sizeY, float angle);
	Bullet(Sprite *shooterSpriteIn, Vector2f targetXY, RenderWindow *inWin, std::string color, int sizeX, int sizeY, float angle, float speed);
	RectangleShape getShape();
	FloatRect getGlobalBounds();
	Vector2f getXY();
	void update();
	void hitEnemy();
	bool isBroken();
	void setSize(int x, int y);
	void start(Vector2f targetXYIn, int size_x = 10, int size_y = 10, int power = 1);
};