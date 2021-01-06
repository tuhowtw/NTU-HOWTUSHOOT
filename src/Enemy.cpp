#include "bullet.h"
#include "Enemy.h"
#include "bullet.h"
#include <math.h>
#include <ctime>
#include <chrono>


using namespace sf;
#define PI 3.1415926

Enemy::Enemy(Vector2f startXY, RenderWindow *inWin, Player *playerIn, Texture *texture, int healthIn, Texture *inExplosionTexture, std::vector<Bullet> *inBulletHolder, int mode) {
	this->mode = mode;
	position = startXY;
	window = inWin;
	player = playerIn;
	health = healthIn;
	explosionTexture = inExplosionTexture;
	bulletHolder = inBulletHolder;
	followCnt = 0;
	chaseWaitTime = 50;
	toStartAfter = false;
	createdTime = std::chrono::steady_clock::now();

	velocity = window->getSize().x / 800 * 0.3;

	windowHeight = window->getSize().x;
	windowWidth = window->getSize().y;

	Color cTemp = enemySprite.getColor();

	if(mode == 2){
		cTemp.g = 255;
	}else{
		cTemp.r += 255;
	}
	enemySprite.setColor(cTemp);

	enemySprite.setTexture(*texture);
	enemySprite.setOrigin(50,50);
	enemySprite.setScale(0.3, 0.3);
	enemySprite.setPosition(position);
	toShow = true;
}

void Enemy::startAfterMiliSec(int milisec, Vector2f playerXY){
	toShow = false;
	startWaitTime = milisec;
	toStartAfter = true;
	bounceTargetXY = playerXY;
}

void Enemy::startMoving(){
	active = true;
	toShow = true;
}

void Enemy::startMoving(Vector2f destXY){
	toShow = true;
	active = true;
	destinationXY = destXY;
	float xd = destinationXY.x - position.x;
	float yd = destinationXY.y - position.y;
	float rate = velocity / pow(pow(xd, 2) + pow(yd, 2), 0.5);
	movingSpeedXY.x = xd * rate;
	movingSpeedXY.y = yd * rate;
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

void Enemy::yellowBullet(){
	useYellowB = true;
}

void Enemy::shoot() {
	Vector2f playerXY = player->getXY();
	if(useYellowB){
		bulletHolder->push_back(Bullet(&enemySprite, playerXY, window, "yellow", 10, 10, -1, 0.6));
	}else{
		bulletHolder->push_back(Bullet(&enemySprite, playerXY, window, "red", 10, 10, -1, 0.6));
	}
	(*bulletHolder)[bulletHolder->size()-1].start(player->getXY(), 5, 5);
	shootWaitTime = 800 + (std::rand()%20)*500;
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
	//whether to start after sec
	if(!active && toStartAfter){
		if(std::chrono::steady_clock::now() - createdTime > std::chrono::milliseconds(startWaitTime)){
			startMoving(bounceTargetXY);
		}
	}

	//check if alive
	if(health <= 0 && !killed){
		killed = true;
		enemySprite.setTexture(*explosionTexture,true);
		enemySprite.setOrigin(250,250);
		enemySprite.setScale(0.15,0.15);
		timeKilled = std::chrono::steady_clock::now();
	}

	//whether to shoot
	if(mode != 2){
		if(active && std::chrono::steady_clock::now() - lastTimeShot > std::chrono::milliseconds(shootWaitTime)){
			shoot();
			lastTimeShot = std::chrono::steady_clock::now();
		}
	}


	//set target
	if(moving == false && killed == false && active){
		moving = true;
		if(mode == 0){
			destinationXY.x = std::rand() % windowWidth;
			destinationXY.y = std::rand() % (windowHeight);
			float xd = destinationXY.x - position.x;
			float yd = destinationXY.y - position.y;
			float rate = velocity / pow(pow(xd, 2) + pow(yd, 2), 0.5);
			movingSpeedXY.x = xd * rate;
			movingSpeedXY.y = yd * rate;
		}else if(mode == 1){
			if(std::chrono::steady_clock::now() - lastTimeChased > std::chrono::milliseconds(chaseWaitTime)){
				destinationXY = player->getXY();
				float xd = destinationXY.x - position.x;
				float yd = destinationXY.y - position.y;
				float rate = velocity / pow(pow(xd, 2) + pow(yd, 2), 0.5);
				movingSpeedXY.x = xd * rate;
				movingSpeedXY.y = yd * rate;
			}
		}
	}

	if(killed && std::chrono::steady_clock::now() - timeKilled > std::chrono::seconds(1)){
		alive = false;
		// std::cout << "dead" <<  std::endl;
	}

	float angle;
	Vector2f playerPosition;
	playerPosition = player->getXY();
	if(moving && !killed){
		if(mode == 0 || mode == 1){
			if(position.x != destinationXY.x || position.y != destinationXY.y){
				float xd = destinationXY.x - position.x;
				float yd = destinationXY.y - position.y;
				float distance = pow(pow(xd, 2) + pow(yd, 2), 0.5);

				if(distance < velocity){
					position = destinationXY;
					moving = false;
				}else{
					//move
					position.x += movingSpeedXY.x;
					position.y += movingSpeedXY.y;
				}
			}else{
				moving = false;
			}
		}else if(mode == 2){
			//reach the bound and bounce
			if(position.x < 0 || position.x > window->getSize().x){
				if(position.x < 0){
					position.x = 1;
				}else{
					position.x = window->getSize().x - 1;
				}
				movingSpeedXY.x *= -1;
			}
			if(position.y < 0 || position.y > window->getSize().y){
				if(position.y < 0){
					position.y = 1;
				}else{
					position.y = window->getSize().y - 1;
				}
				movingSpeedXY.y *= -1;
			}

			destinationXY.x = position.x + movingSpeedXY.x * 1000;
			destinationXY.y = position.y + movingSpeedXY.y * 1000;
			position += movingSpeedXY;
		}
	}
	//face the player/ destination
	Vector2f targetXY;
	if(mode == 0 || mode == 3){
		targetXY = player->getXY();
	}else{
		if(!active){
			targetXY = player->getXY();
		}else{
			targetXY = destinationXY;
		}
	}
	float x = targetXY.x - position.x;
	float y = targetXY.y - position.y;

	if(!killed){
		angle = -atan2(x, y) * 180 / PI;
		enemySprite.setRotation(angle);
	}
	enemySprite.setPosition(position);
	if(toShow) window->draw(enemySprite);
}