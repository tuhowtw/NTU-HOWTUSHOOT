#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <ctime>
using namespace sf;

class Enemy {
private:
	Sprite enemySprite;
	Texture enemyTexture;
	Texture *explosionTexture;
	Vector2f position;
	Vector2f defaultPosition;
	Player *player;
	RenderWindow *window;
	int windowHeight, windowWidth;
	float velocity = 0.5;
	bool moving = false;
	Vector2f destinationXY;
	Vector2f movingSpeedXY;
	bool killed = false;
	bool alive = true;
	int health;
	bool active = false;
	std::chrono::steady_clock::time_point lastTimeShot;
	int shootWaitTime;
	std::chrono::steady_clock::time_point timeKilled;
	bool hasShotFirst = false;
	std::vector<Bullet> *bulletHolder;



public:
	Enemy(Vector2f startXY, RenderWindow *inWin, Player *playerIn, Texture *texture, int healthIn, Texture *explosionTexture, std::vector<Bullet> *inBulletHolder);
	// Enemy(Vector2f startXY, RenderWindow *inWin, Player *playerIn, Texture *texture, int healthIn, Texture *explosionTexture, std::vector<Bullet> *inBulletHolder);
	FloatRect getGlobalBounds();
	Sprite * getSprite();
	Vector2f getXY();

	//void moveToPoint();
	void update();
	void startMoving();
	void shoot();
	void getHit(int damage);
	bool isAlive();
	bool isKilled();
};