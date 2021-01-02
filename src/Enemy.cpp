#include "bullet.h"
#include "Enemy.h"
#include "bullet.h"
#include <math.h>
#include <ctime>
#include <chrono>


using namespace sf;
#define PI 3.1415926

Enemy::Enemy(Vector2f startXY, RenderWindow *inWin, Player *playerIn, Texture *texture, int healthIn, Texture *inExplosionTexture, std::vector<Bullet> *inBulletHolder) {
	position = startXY;
	window = inWin;
	player = playerIn;
	health = healthIn;
	explosionTexture = inExplosionTexture;
	bulletHolder = inBulletHolder;

	velocity = window->getSize().x / 800 * 0.3;

	windowHeight = window->getSize().x;
	windowWidth = window->getSize().y;

	enemySprite.setTexture(*texture);
	enemySprite.setOrigin(50,50);
	enemySprite.setScale(0.3, 0.3);
	enemySprite.setPosition(position);
}

void Enemy::startMoving(){
	active = true;
}

FloatRect Enemy::getGlobalBounds() {
	//float unit = 17.5;
	return enemySprite.getGlobalBounds();
	//return FloatRect(position.x - unit, position.y - unit, position.x + unit, position.y + unit);
}

Vector2f Enemy::getXY() {
	return position;
}

Sprite * Enemy::getSprite() {
	return &enemySprite;
}

//void Enemy::moveToPoint() {
//	toPointX = rd() % windowWidth;
//	toPointY = rd() % windowHeight;
//
//	int xd = toPointX - position.x;
//	int yd = toPointY - position.y;
//
//	float rate = velocity / pow(pow(xd, 2) + pow(yd, 2), 0.5);
//	xMove = xd * rate;
//	yMove = yd * rate;
//	moving = true;
//
//}

void Enemy::shoot() {
	Vector2f playerXY = player->getXY();
	bulletHolder->push_back(Bullet(&enemySprite, playerXY, window, "red", 10, 10, -1, 0.1));
	(*bulletHolder)[bulletHolder->size()-1].start(player->getXY(), 3, 3);
	shootWaitTime = 200 + (std::rand()%20)*50;
}

void Enemy::getHit(int damage){
	health -= damage;
}

bool Enemy::isAlive(){
	return alive;
}

bool Enemy::isKilled(){
	return killed;
}

void Enemy::update() {
	//killed = true;
	//check if alive
	if(health <= 0 && !killed){
		killed = true;
		enemySprite.setTexture(*explosionTexture,true);
		enemySprite.setOrigin(250,250);
		enemySprite.setScale(0.15,0.15);
		timeKilled = std::chrono::steady_clock::now();
	}

	//whether to shoot
	if(!hasShotFirst && active){
		hasShotFirst = true;
		shoot();
		lastTimeShot = std::chrono::steady_clock::now();
	}else if(active && std::chrono::steady_clock::now() - lastTimeShot > std::chrono::milliseconds(shootWaitTime)){
		shoot();
		lastTimeShot = std::chrono::steady_clock::now();
	}

	//whether to start moving
	if(moving == false && killed == false && active){
		moving = true;
		destinationXY.x = std::rand()%windowWidth;
		destinationXY.y = std::rand()%(windowHeight);
		float xd = destinationXY.x - position.x;
		float yd = destinationXY.y - position.y;
		float rate = velocity / pow(pow(xd, 2) + pow(yd, 2), 0.5);
		movingSpeedXY.x = xd * rate;
		movingSpeedXY.y = yd * rate;
	}

	if(killed && std::chrono::steady_clock::now() - timeKilled > std::chrono::seconds(1)){
		alive = false;
		std::cout << "dead" <<  std::endl;
	}

	float angle;
	Vector2f playerPosition;
	playerPosition = player->getXY();
	if(moving && !killed){
		if(position.x != destinationXY.x || position.y != destinationXY.y){
			float xd = destinationXY.x - position.x;
			float yd = destinationXY.y - position.y;
			float distance = pow(pow(xd, 2) + pow(yd, 2), 0.5);

			if(distance < velocity){
				position = destinationXY;
				moving = false;
			}else{
				//move
				// position.x += movingSpeedXY.x;
				// position.y += movingSpeedXY.y;
			}

		}else{
			moving = false;
		}
	}


	////move to a point
	//if (moving) {
	//	position.x += xMove;
	//	position.y += yMove;

	//	if (position.x > toPointX - 2 && position.x < toPointX + 2 && position.y > toPointY - 2 && position.y < toPointY + 2) {
	//		moving = false;
	//	}
	//}

	//face the player
	float x = playerPosition.x - position.x;
	float y = playerPosition.y - position.y;

	if(!killed){
		angle = -atan2(x, y) * 180 / PI;
		enemySprite.setRotation(angle);
	}


	enemySprite.setPosition(position);
	window->draw(enemySprite);
}