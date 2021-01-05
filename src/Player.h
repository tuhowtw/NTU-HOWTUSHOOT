#pragma once
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;


class Player {
private:
	Sprite playerSprite;
	Texture playerTexture;

	Sprite shieldSprite;
	Texture shieldTexture;
	Vector2f shieldPosition;


	bool shieldOn = true;
	std::chrono::steady_clock::time_point shieldStartT;
	std::chrono::steady_clock::time_point shieldLastThinnedT;
	int shieldColorCnt;
	int shieldDuration;

	Texture explosionTexture;
	Vector2f position;
	Vector2f defaultPosition;
	RenderWindow *window;
	float xVelocity = 0.3;
	float yVelocity = 0.3;
	float mouseX;
	float mouseY;
	float windowWidth;
	float windowHeight;
	int health = 5;
	bool killed = false;
	bool alive = true;
	std::chrono::steady_clock::time_point timeKilled;

public:
	Player(float startX, float startY, RenderWindow *inWin);
	FloatRect getGlobalBounds();
	FloatRect getShieldBounds();

	Sprite * getSprite();
	Vector2f getXY();

	bool inBound();
	bool isAlive();
	int getLife();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void startShield(int sec = 5);
	bool isShieldOn();
	void update();
	void shoot();
	void getHit(int damage);

};