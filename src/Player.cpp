#include "Player.h"
#include <chrono>
#include <ctime>
#define PI 3.1415926
using namespace sf;

Player::Player(float startX, float startY, RenderWindow *inWin) {
	position.x = startX;
	position.y = startY;
	window = inWin;
	windowWidth = window->getSize().x;
	windowHeight = window->getSize().y;
	xVelocity = window->getSize().x/800 * 0.5;
	yVelocity = xVelocity;

	defaultPosition = position;

	explosionTexture.loadFromFile("Textures/explosion.png");
	playerTexture.loadFromFile("Textures/player_100x100.png");
	shieldTexture.loadFromFile("Textures/shield.png");

	playerSprite.setTexture(playerTexture);
	playerSprite.setOrigin(50,50);
	playerSprite.setScale(0.2, 0.2);
	playerSprite.setPosition(position);

	shieldSprite.setTexture(shieldTexture);
	shieldSprite.setOrigin(50,50);
	shieldSprite.setScale(1, 1);
	shieldSprite.setPosition(position);
}

FloatRect Player::getGlobalBounds() {
	return playerSprite.getGlobalBounds();
}

FloatRect Player::getShieldBounds(){
	return shieldSprite.getGlobalBounds();
}

Vector2f Player::getXY() {
	return position;
}

Sprite * Player::getSprite() {
	return &playerSprite;
}

void Player::moveUp() {
	if(position.y - yVelocity > 0) position.y -= yVelocity;
}

void Player::moveDown() {
	if(position.y + yVelocity < window->getSize().y) position.y += yVelocity;
}

void Player::moveLeft() {
	if(position.x - xVelocity > 0) position.x -= xVelocity;
}

void Player::moveRight() {
	if(position.x + xVelocity < window->getSize().x) position.x += xVelocity;
}

bool Player::inBound() {
	if (position.x < 0 || position.y < 0 || position.x -50 > window->getSize().x || position.y - 50 > window->getSize().y) {
		return false;
	}
	else {
		return true;
	}
}

bool Player::isAlive(){
	return alive;
}

int Player::getLife(){
	return health;
}

void Player::update() {
	//whether alive
	if(health <= 0 && !killed){
		killed = true;
		playerSprite.setTexture(explosionTexture,true);
		playerSprite.setOrigin(250,250);
		playerSprite.setScale(0.15,0.15);
		timeKilled = std::chrono::steady_clock::now();
	}

	if(killed){
		auto deadSeconds = std::chrono::steady_clock::now() - timeKilled;
		if(deadSeconds > std::chrono::seconds(2)){
			alive = false;
		}
	}

	float angle;
	Vector2i mousePosition;
	mousePosition = Mouse::getPosition(*window);
	float x = mousePosition.x - position.x;
	float y = mousePosition.y - position.y;

	angle = -atan2(x, y) * 180 / PI;
	playerSprite.setRotation(angle);
	playerSprite.setPosition(position);
	shieldSprite.rotate(1);
	shieldSprite.setPosition(position);
	window->draw(playerSprite);
	if(shieldOn){
		if(std::chrono::steady_clock::now() - shieldStartT < std::chrono::seconds(shieldDuration)){
			if(std::chrono::steady_clock::now() - shieldLastThinnedT > std::chrono::milliseconds(500)){
				Color shieldColor = shieldSprite.getColor();
				if(shieldColorCnt > 1){
					shieldColor.a -= 255.0 / (shieldDuration * 2);
					shieldSprite.setColor(shieldColor);
					shieldColorCnt--;
				}
				shieldLastThinnedT = std::chrono::steady_clock::now();
			}

			window->draw(shieldSprite);
		}else{
			shieldOn = false;
		}

	}
}

void Player::getHit(int damage){
	health -= damage;
	startShield();
}


void Player::startShield(int sec){
	shieldDuration = sec;
	shieldOn = true;
	Color shieldColor = shieldSprite.getColor();
	shieldColor.a = 255;
	shieldSprite.setColor(shieldColor);
	shieldColorCnt = sec * 2;
	shieldStartT = std::chrono::steady_clock::now();
}

bool Player::isShieldOn(){
	return shieldOn;
}
