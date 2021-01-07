#include <SFML/Graphics.hpp>
#include "bullet.h"
#include <string>
#include <math.h>
#define PI 3.1415926

using namespace sf;

Sound Bullet::bounceSound;

Bullet::Bullet(Sprite *shooterSpriteIn, Vector2f targetXYIn, RenderWindow *inWin, std::string color, int sizeX, int sizeY, float angle) {
	window = inWin;
	xSize = sizeX;
	ySize = sizeY;
	shooterSprite = shooterSpriteIn;
	targetXY = targetXYIn;
	health = 1;
	bounceCnt = 0;
	toBounce = false;
	toRemoteCharge = false;

	shooterXY = shooterSprite->getPosition();
	movingSpeed = window->getSize().x / 800 * 2;
	bulletShape.setSize(sf::Vector2f(xSize,ySize));
	bulletShape.setOrigin(sizeX/2,sizeY/2);
	if(color == "white"){
		bulletColor = Color::White;
	}else if(color == "red"){
		bulletColor = Color::Red;
	}else if(color == "yellow"){
		bulletColor = Color::Yellow;
	}
	bulletShape.setFillColor(bulletColor);



	float xd = targetXY.x - shooterXY.x;
	float yd = targetXY.y - shooterXY.y;

	float angle2;
	angle2 = -atan2(xd, yd) * 180 / PI;
	bulletShape.setRotation(angle2);


	/* Vector2i mousePosition = Mouse::getPosition(*window); */
	position = shooterXY;
	if(angle != -1){
		Vector2f center = shooterSprite->getPosition();
		position.x = position.x + 50;
		//rotated
		position.x = cos(angle) * (position.x - center.x) - sin(angle) * (position.y-center.y) + center.x;
		position.y = sin(angle) * (position.x - center.x) + cos(angle) * (position.y - center.y) + center.y;
		float angle;
		Vector2i mousePosition = Mouse::getPosition(*window);
		float xd = mousePosition.x - position.x;
		float yd = mousePosition.y - position.y;
		angle = -atan2(xd, yd) * 180 / PI;
		transform.rotate(angle, center);
	}

	bulletShape.setPosition(position);
}

Bullet::Bullet(Sprite *shooterSpriteIn, Vector2f targetXYIn, RenderWindow *inWin, std::string color, int sizeX, int sizeY, float angle, float speed) {
	window = inWin;
	xSize = sizeX;
	ySize = sizeY;
	shooterSprite = shooterSpriteIn;
	targetXY = targetXYIn;
	speedUnit = speed;
	health = 1;
	bounceCnt = 0;
	toBounce = false;
	toRemoteCharge = false;

	shooterXY = shooterSprite->getPosition();
	movingSpeed = window->getSize().x / 800 * speedUnit;
	bulletShape.setSize(sf::Vector2f(xSize,ySize));
	bulletShape.setOrigin(sizeX/2,sizeY/2);
	if(color == "white"){
		bulletShape.setFillColor(Color::White);
	}else if(color == "red"){
		bulletShape.setFillColor(Color::Red);
	}else if(color == "yellow"){
		bulletShape.setFillColor(Color::Yellow);
	}


	float xd = targetXY.x - shooterXY.x;
	float yd = targetXY.y - shooterXY.y;

	float angle2;
	angle2 = -atan2(xd, yd) * 180 / PI;
	bulletShape.setRotation(angle2);


	/* Vector2i mousePosition = Mouse::getPosition(*window); */
	position = shooterXY;
	if(angle != -1){
		Vector2f center = shooterSprite->getPosition();
		position.x = position.x + 50;
		//rotated
		position.x = cos(angle) * (position.x - center.x) - sin(angle) * (position.y-center.y) + center.x;
		position.y = sin(angle) * (position.x - center.x) + cos(angle) * (position.y - center.y) + center.y;
		float angle;
		Vector2i mousePosition = Mouse::getPosition(*window);
		float xd = mousePosition.x - position.x;
		float yd = mousePosition.y - position.y;
		angle = -atan2(xd, yd) * 180 / PI;
		transform.rotate(angle, center);
	}

	bulletShape.setPosition(position);
}


RectangleShape Bullet::getShape() {
	return bulletShape;
}

void Bullet::update() {
	bulletShape.setOrigin(bulletShape.getSize().x/2,bulletShape.getSize().y/2);
	transform = Transform();


	if(active){
		position.x += xMove;
		position.y += yMove;
	}else{
		if(!toRemoteCharge){
			float angle;
			Vector2i mousePosition = Mouse::getPosition(*window);
			position = shooterSprite->getPosition();
			float xd = mousePosition.x - position.x;
			float yd = mousePosition.y - position.y;
			angle = -atan2(xd, yd) * 180 / PI;
			Vector2f center = shooterSprite->getPosition();
			transform.rotate(angle, center);
			//rotated
		}

	}
	if(!broken && toBounce){
		if(bounceCnt < 10){
			if(position.x < 0 || position.x > window->getSize().x){
				if(position.x < 0){
					position.x = 1;
				}else{
					position.x = window->getSize().x - 1;
				}
				xMove *= -1;
				bounceSound.play();
				bounceCnt++;
			}
			if(position.y < 0 || position.y > window->getSize().y){
				if(position.y < 0){
					position.y = 1;
				}else{
					position.y = window->getSize().y - 1;
				}
				yMove *= -1;
				bounceSound.play();
				bounceCnt++;
			}
		}
	}


	bulletShape.setPosition(position);

	if(!broken){
		if(active){
			window->draw(bulletShape);
		}else{
			window->draw(bulletShape, transform);
		}
	}

}

void Bullet::setSize(int x, int y){
	bulletShape.setSize(Vector2f(x,y));
}

Vector2f Bullet::getXY() {
	return position;
}

FloatRect Bullet::getGlobalBounds() {
	return bulletShape.getGlobalBounds();
}

void Bullet::hitEnemy(){
	health -= 2;
	if(health <= 0){
		broken = true;
		movingSpeed = 1000000;
	}
}

bool Bullet::isBroken(){
	return broken;
}

void Bullet::start(Vector2f targetXYIn, int size_x , int size_y, int power){
	health = power;
	active = true;
	targetXY = targetXYIn;
	bulletShape.setSize(Vector2f(size_x,size_y));
	xd = targetXY.x - position.x;
	yd = targetXY.y - position.y;
	rate = movingSpeed / pow(pow(xd, 2) + pow(yd, 2), 0.5);
	xMove = xd * rate;
	yMove = yd * rate;
}

void Bullet::setHealth(int h){
	health = h;
	if(h > 90){
		toBounce = true;
	}
}

void Bullet::setRemoteCharge(){
	toRemoteCharge = true;
}

void Bullet::setBounceSound(Sound s){
	bounceSound = s;
}

