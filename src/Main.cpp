// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <SFML/Graphics.hpp>
#include "EnemyQuadtree.h"
#include "BulletQuadtree.h"
#include "Main.hpp"
#include "Player.h"
#include "bullet.h"
#include "Enemy.h"
#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
using namespace sf;

#define GAMEMENU 1
#define GAMESTART 2
#define GAMEOVER 3
#define PI 3.1415926

const bool PLAYER_NOT_INVINCIBLE = 1, GRID_ON = 1, GRID_ACTIVE = 1, LINE_ON = 1, BLOCK_ON = 1;
const bool BULLET_INVINCIBLE = 1, KEEP_GAME = 0;
const int GRID_MODE = 2;

int main(){
	//for controlling process
	srand(time(NULL));

	bool enemyWork = true;
	bool playerWork = true;


	int GAMESTATE = GAMEMENU;
	bool win = false;

	int windowWidth = 1920;
	int windowHeight = 1080;

	BulletQuadtree enemyBulletTree(0, 0, windowWidth, windowHeight, 0, 5);
	BulletQuadtree playerBulletTree(0, 0, windowWidth, windowHeight, 0, 5);

	Vector2f windowSize;
	windowSize.x = windowWidth;
	windowSize.y = windowHeight;
	RenderWindow *window;
	window = new RenderWindow(sf::VideoMode(windowWidth, windowHeight), "HOWTU SHOOOOT!");

	Font font;
	font.loadFromFile("Font/Nikolas & Pine.ttf");



	Text gameTitle;
	gameTitle.setFont(font);
	gameTitle.setString("HOWTU SHOOT");
	gameTitle.setCharacterSize(64);
	float origin_x = gameTitle.getLocalBounds().width/2;
	float origin_y = gameTitle.getLocalBounds().height/2;
	gameTitle.setOrigin(origin_x,origin_y);
	gameTitle.setPosition(windowWidth/2,windowHeight/2);

	Text promptTxt;
	promptTxt.setFont(font);
	promptTxt.setString("Press Space to Start...");
	promptTxt.setCharacterSize(32);
	origin_x = promptTxt.getLocalBounds().width/2;
	origin_y = promptTxt.getLocalBounds().height/2;
	promptTxt.setOrigin(origin_x,origin_y);
	promptTxt.setPosition(windowWidth/2,windowHeight/2+100);

	Text ggTxt;
	ggTxt.setFont(font);
	ggTxt.setCharacterSize(32);

	Text objectNumTxt;
	objectNumTxt.setFont(font);
	objectNumTxt.setCharacterSize(24);
	objectNumTxt.setPosition(window->getSize().x - 200, window->getSize().y - 300);

	Text lifeCountTxt;
	lifeCountTxt.setFont(font);
	lifeCountTxt.setString("Life: ");
	lifeCountTxt.setCharacterSize(24);

	lifeCountTxt.setPosition(window->getSize().x-200,window->getSize().y-200);

	Text pointTxt;
	pointTxt.setFont(font);
	pointTxt.setString("Point: ");
	pointTxt.setCharacterSize(24);
	pointTxt.setPosition(window->getSize().x-200,window->getSize().y-200+24);

	Text shieldCountTxt;
	shieldCountTxt.setFont(font);
	shieldCountTxt.setCharacterSize(24);
	shieldCountTxt.setString("Shield Count: ");
	shieldCountTxt.setPosition(window->getSize().x-200,window->getSize().y-200+48);

	Text superShieldRechargeTimeTxt;
	superShieldRechargeTimeTxt.setFont(font);
	superShieldRechargeTimeTxt.setCharacterSize(24);
	superShieldRechargeTimeTxt.setString("Super Shield Cooldown Time: ");
	superShieldRechargeTimeTxt.setPosition(window->getSize().x-200,window->getSize().y-200 + 72);

	//audio
	SoundBuffer b1, b2, b3, b4, b5, b6, b7, b8, bounceBuffer;
	Sound shootSound, hitSound, winSound, loseSound, shieldSound, hitShieldSound, startSound, enemySound, bounceSound;
	b1.loadFromFile("Audio/shoot.wav");
	shootSound.setBuffer(b1);
	b2.loadFromFile("Audio/hit.wav");
	hitSound.setBuffer(b2);
	b3.loadFromFile("Audio/win.wav");
	winSound.setBuffer(b3);
	b4.loadFromFile("Audio/lose.wav");
	loseSound.setBuffer(b4);
	b5.loadFromFile("Audio/shield.wav");
	shieldSound.setBuffer(b5);
	b6.loadFromFile("Audio/hitShield.wav");
	hitShieldSound.setBuffer(b6);
	b7.loadFromFile("Audio/start.wav");
	startSound.setBuffer(b7);
	b8.loadFromFile("Audio/start.wav");
	enemySound.setBuffer(b8);
	bounceBuffer.loadFromFile("Audio/bounce.wav");
	bounceSound.setBuffer(bounceBuffer);
	Bullet::setBounceSound(bounceSound);


	bool resultSoundPlayed = false;


	Player *player = new Player(windowWidth / 2, windowHeight / 2 + 200 , window);
	int playerBulletPower = 1;
	std::chrono::steady_clock::time_point bulletPowerT;
	std::vector<Bullet> playerBulletHolder;
	std::vector<Bullet> playerShieldHolder;
	int playerShieldCount = 10;
	std::chrono::steady_clock::time_point lastShieldAddedT;
	std::chrono::steady_clock::time_point lastShieldT;
	std::chrono::steady_clock::time_point playerLastShootTime;
	const int CHARGE_MODE = 0, REMOTE_CHARGE_MODE = 1, MACHINE_GUN_MODE = 2;
	int playerShootingMode = CHARGE_MODE;
	int playerShootInterval = 10;

	std::chrono::steady_clock::time_point lastSuperShieldT;
	std::chrono::seconds superShieldInterval(10);

	std::vector<Bullet> *enemyBulletHolder = new std::vector<Bullet>;
	int playerPoint = 0;

	std::chrono::steady_clock::time_point playerLastHitTime;


	Texture *enemyTexture = new Texture();
	enemyTexture->loadFromFile("Textures/enemy.png");
	Texture *enemyLineTexture = new Texture();
	enemyLineTexture->loadFromFile("Textures/enemy2.png");
	Texture *enemyTexture3 = new Texture();
	enemyTexture3->loadFromFile("Textures/enemy3.png");
	Texture *enemyTexture4 = new Texture();
	enemyTexture4->loadFromFile("Textures/enemy4.png");
	Texture *enemyTexture5 = new Texture();
	enemyTexture5->loadFromFile("Textures/enemy5.png");
	Texture *explosionTexture = new Texture();
	explosionTexture->loadFromFile("Textures/explosion.png");




	//only single click counts
	bool mouseLPressed = false;
	bool mouseRPressed = false;


	//float lastShotTime = -1;


	//std::clock_t start = std::clock();
/* 	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			enemyXY.x = enemyStartPoint.x + i * 20;
			enemyXY.y = enemyStartPoint.y + j * 20;
			enemyHolder.push_back(Enemy(enemyXY, window, player, enemyTexture, 1, explosionTexture));
		}

	} */

	//std::chrono::duration<double> elapsed_seconds = enemyCreatedT - std::chrono::steady_clock::now();
	bool enemyStarted = false;
	// bool lineCreated = false;
	// bool lineStarted = false;
	// Enemy* lineMembers[10];
	bool created = false;
	std::chrono::steady_clock::time_point enemyCreatedT;



	//for spawning enemies
	int i = 0;
	int j = 0;
	int count = 0;
	bool spawnStart = false;
	std::chrono::steady_clock::time_point lastCreatedTime;

	//turret block
	int turretI = 0, turretJ = 0, turretCnt = 0, blockWaitTime = 5000, blockLatency = 42;
	int blockSize = 0, blockMode = 0;
	bool turretStart = false;
	Vector2f blockStartXY;
	std::chrono::steady_clock::time_point lastBlockT, lastTurretT;
	int blockCnt = 0, lineCnt = 0;
	int lineWaitTime = 5000;

	int GRID_SIZE = 11;


	Vector2f enemyXY;
	Vector2f enemyStartPoint;
	int enemyInterval = 40;
	std::vector<Enemy> enemyHolder;
	std::chrono::steady_clock::time_point lastSpawnT;
/*
	bool lineStarting = false;
	Vector2f lineTargetXY;
	Enemy* lineMembers[10]; */
	// int lineMemberStartLatency = 0;
	std::chrono::steady_clock::time_point lastLineStartT;

	while (window->isOpen()){
		if(!created){
			enemyStartPoint.x = window->getSize().x / 2 - (GRID_SIZE / 2) * enemyInterval;
			enemyStartPoint.y = window->getSize().y / 2 - (GRID_SIZE / 2) * enemyInterval;
		}
		Event event;
		while (window->pollEvent(event)) {
			if (event.type == Event::Closed) window->close();
		}
		if (event.type == sf::Event::Resized){
			// update the view to the new size of the window
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			window->setView(sf::View(visibleArea));
		}
		window->clear();

		if(GAMESTATE == GAMEMENU){
			gameTitle.setPosition(window->getSize().x/2,window->getSize().y/2);
			promptTxt.setPosition(window->getSize().x/2,window->getSize().y/2+100);
			window->draw(gameTitle);
			window->draw(promptTxt);
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				window->clear();
				startSound.play();
				GAMESTATE = GAMESTART;
			}


		}

		if(GAMESTATE == GAMESTART){

			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				playerWork = !playerWork;
			}
			if(GRID_ON &&!created){
				if(!spawnStart){
					if(std::chrono::steady_clock::now() - lastCreatedTime > std::chrono::milliseconds(5)){
						if(i < GRID_SIZE){
							if(j < GRID_SIZE){
								enemyXY.x = enemyStartPoint.x + i * enemyInterval;
								enemyXY.y = enemyStartPoint.y + j * enemyInterval;
								enemyHolder.push_back(Enemy(enemyXY, window, player, enemyTexture3, 1, explosionTexture, enemyBulletHolder, GRID_MODE));
								lastCreatedTime = std::chrono::steady_clock::now();
								enemySound.play();
								count++;
								j++;
							}
							if(j == GRID_SIZE){
								j = 0;
								i++;
							}
						}
					}
					if(count >= GRID_SIZE * GRID_SIZE){
						created = true;
						enemyCreatedT = std::chrono::steady_clock::now();
					}
				}
			}

			if(!enemyWork){
				created = false;
			}

			if(!GRID_ON) created = true;

			if(BLOCK_ON && created && !turretStart && std::chrono::steady_clock::now() - lastBlockT > std::chrono::milliseconds(blockWaitTime)){
				lastBlockT = std::chrono::steady_clock::now();
				blockWaitTime = 2000 - blockCnt * 500;
				if(blockWaitTime < 500) blockWaitTime = 500;
				int rand = std::rand() % 2;
				if(rand == 0) blockMode = 2;
					else blockMode = 3;
				turretStart = true;
				blockStartXY.x = std::rand() % 1000 + window->getSize().x / 2 - 500;
				blockStartXY.y = std::rand() % 1000 + window->getSize().y / 2 - 500;
				// blockStartXY.x = std::rand() % window->getSize().x;
				// blockStartXY.y = std::rand() % window->getSize().y;
				blockSize = std::rand() % 2 + 3;
				turretI = 0;
				turretJ = 0;
				blockCnt++;
			}

			if(BLOCK_ON && turretStart){
				if(std::chrono::steady_clock::now() - lastTurretT > std::chrono::milliseconds(5)){
					if(turretI < blockSize){
						if(turretJ < blockSize){
							Vector2f xy;
							xy.x = blockStartXY.x + turretI * enemyInterval;
							xy.y = blockStartXY.y + turretJ * enemyInterval;
							if(blockMode == 2){
								enemyHolder.push_back(Enemy(xy, window, player, enemyTexture4, 1, explosionTexture, enemyBulletHolder, blockMode));
							}else{
								enemyHolder.push_back(Enemy(xy, window, player, enemyTexture5, 1, explosionTexture, enemyBulletHolder, blockMode));
								enemyHolder[enemyHolder.size() - 1].yellowBullet();
							}
							enemyHolder[enemyHolder.size() - 1].startAfterMiliSec((blockLatency * turretCnt), player->getXY());
							lastTurretT = std::chrono::steady_clock::now();
							enemySound.play();
							turretCnt++;
							turretJ++;
						}
						if(turretJ == blockSize){
							turretJ = 0;
							turretI++;
						}
					}
					if(turretCnt >= blockSize * blockSize){
						turretStart = false;
						turretCnt = 0;
					}
				}
			}



			//created = false;
			//wait for 2 sec then enemies move
			auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - enemyCreatedT);
			if(GRID_ON && GRID_ACTIVE && elapsed_seconds > std::chrono::seconds(0) && !enemyStarted && created){
				enemyStarted = true;
				std::vector<Enemy>::iterator itStart = enemyHolder.begin();
				while (itStart != enemyHolder.end()){
					itStart->startMoving(player->getXY());
					itStart++;
				}
			}

			if(LINE_ON && created && std::chrono::steady_clock::now() - lastCreatedTime > std::chrono::milliseconds(lineWaitTime)){
				lastCreatedTime = std::chrono::steady_clock::now();
				lineWaitTime = 5000 - lineCnt * 500;
				if(lineWaitTime < 1500) lineWaitTime = 1500;
				Vector2f enemyXY, playerXY;
				enemyXY.x = std::rand()%window->getSize().x;
				enemyXY.y = std::rand()%window->getSize().y;
				playerXY = player->getXY();
				int startLatency = 0;
				for(int i = 0; i < 20; i++){
					startLatency += 150;
					enemyHolder.push_back(Enemy(enemyXY, window, player, enemyLineTexture, 1, explosionTexture, enemyBulletHolder, 2));
					enemyHolder[enemyHolder.size() - 1].startAfterMiliSec(startLatency, playerXY);
					// lineMembers[i] = &enemyHolder[enemyHolder.size() - 1];
				}
				enemySound.play();
				lineCnt++;
			}





			//Player moving
			if (Keyboard::isKeyPressed(Keyboard::W) && player->getXY().y - 1 > 0) {
				player->moveUp();
			}
			else if (Keyboard::isKeyPressed(Keyboard::A) && player->getXY().x - 1 >0){
				player->moveLeft();
			}
			else if (Keyboard::isKeyPressed(Keyboard::S) && player->getXY().y + 1 < window->getSize().y) {
				player->moveDown();
			}
			else if (Keyboard::isKeyPressed(Keyboard::D) && player->getXY().x +1 < window->getSize().x) {
				player->moveRight();
			}

			if(Keyboard::isKeyPressed(Keyboard::Num1)){
				playerShootingMode = CHARGE_MODE;
			}

			if(Keyboard::isKeyPressed(Keyboard::Num2)){
				playerShootingMode = REMOTE_CHARGE_MODE;
			}

			if(Keyboard::isKeyPressed(Keyboard::Num3)){
				playerShootingMode = MACHINE_GUN_MODE;
			}

			//trigger
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if(playerShootingMode == CHARGE_MODE || playerShootingMode == REMOTE_CHARGE_MODE){
					if(!mouseLPressed){
						bulletPowerT = std::chrono::steady_clock::now();
						float angle;
						Vector2f mouseXY;
						Vector2i iMouseXY = Mouse::getPosition(*window);
						mouseXY.x = iMouseXY.x;
						mouseXY.y = iMouseXY.y;
						Vector2f playerXY_temp = player->getXY();
						float xd = mouseXY.x - playerXY_temp.x;
						float yd = mouseXY.y - playerXY_temp.y;
						angle = -atan2(xd, yd) * 180 / PI;

						playerBulletHolder.push_back(Bullet(player->getSprite(), mouseXY, window, "white", 5 * playerBulletPower, 5 * playerBulletPower, angle));
						if(playerShootingMode == REMOTE_CHARGE_MODE) playerBulletHolder[playerBulletHolder.size() - 1].setRemoteCharge();
						//lastShotTime = std::clock();
					}else if(std::chrono::steady_clock::now() - bulletPowerT > std::chrono::milliseconds(5)){
						bulletPowerT = std::chrono::steady_clock::now();
						if(playerBulletPower < 50){
							playerBulletPower += 1;
						}
						playerBulletHolder[playerBulletHolder.size() - 1].setSize(3 * playerBulletPower, 3 * playerBulletPower);
						playerBulletHolder[playerBulletHolder.size() - 1].setHealth(playerBulletPower * 2);
					}

					mouseLPressed = true;
				}else if(playerShootingMode == MACHINE_GUN_MODE){
					shootSound.setVolume(10);
					Vector2f mouseXY;
					Vector2i iMouseXY = Mouse::getPosition(*window);
					mouseXY.x = iMouseXY.x;
					mouseXY.y = iMouseXY.y;
					if(std::chrono::steady_clock::now() - playerLastShootTime > std::chrono::milliseconds(playerShootInterval)){
						playerBulletHolder.push_back(Bullet(player->getSprite(), mouseXY, window, "white", 3 * playerBulletPower, 3 * playerBulletPower, -1));
						playerBulletHolder[playerBulletHolder.size()-1].start(mouseXY, 3 * playerBulletPower, 3 * playerBulletPower, playerBulletPower);
						shootSound.play();
						playerLastShootTime = std::chrono::steady_clock::now();
					}
				}


			}
			if (Mouse::isButtonPressed(Mouse::Right)) {
				if(!mouseRPressed){
					mouseRPressed = true;
					if(playerShieldCount > 0){
						playerShieldCount -= 1;
						Vector2f mouseXY;
						Vector2i iMouseXY = Mouse::getPosition(*window);
						mouseXY.x = iMouseXY.x;
						mouseXY.y = iMouseXY.y;
						playerShieldHolder.push_back(Bullet(player->getSprite(), mouseXY, window, "yellow", 100, 10 ,-1));
						playerShieldHolder[playerShieldHolder.size()-1].start(mouseXY, 100, 10);
					}
				}
					//lastShotTime = std::clock();
			}

			if(Keyboard::isKeyPressed(Keyboard::F)){
				if(std::chrono::steady_clock::now() - lastSuperShieldT > superShieldInterval){
					shieldSound.play();
					player->startShield();
					lastSuperShieldT = std::chrono::steady_clock::now();
				}
			}

			//recharge shieldcount
			if(std::chrono::steady_clock::now() - lastShieldAddedT > std::chrono::seconds(1)){
				if(playerShieldCount < 10){
					lastShieldAddedT = std::chrono::steady_clock::now();
					playerShieldCount += 1;
				}
			}



			if(Mouse::isButtonPressed(Mouse::Left) == false){
				if(mouseLPressed){
					Vector2f mouseXY;
					Vector2i iMouseXY = Mouse::getPosition(*window);
					mouseXY.x = iMouseXY.x;
					mouseXY.y = iMouseXY.y;
					playerBulletHolder[playerBulletHolder.size()-1].start(mouseXY, 3 * playerBulletPower, 3 * playerBulletPower, playerBulletPower);
					shootSound.setVolume(100);
					shootSound.play();
				}
				mouseLPressed = false;
				playerBulletPower = 1;
			}

			if(Mouse::isButtonPressed(Mouse::Right) == false){
				mouseRPressed = false;
			}


			for(int i = 0; i < static_cast<int>(enemyBulletHolder->size()); i++){
				enemyBulletTree.AddObject(&(*enemyBulletHolder)[i]);
			}

			for(int i = 0; i < static_cast<int>(playerBulletHolder.size()); i++){
				playerBulletTree.AddObject(&playerBulletHolder[i]);
			}


			//judge if bullets hit enemies
			/* for(int i = 0; i < static_cast<int>(enemyHolder.size()); i++){
				if(enemyHolder[i].isKilled() || !created) continue;
				Vector2f _xy = enemyHolder[i].getXY();
				vector<Bullet *> returnObjs = playerBulletTree.GetObjectsAt(_xy.x, _xy.y);
				// cout << "Size:" << returnObjs.size() << endl;
				for(int j = 0; j < static_cast<int>(returnObjs.size()); j++){
					if(returnObjs[j]->isBroken()) continue;
					if(returnObjs[j]->getGlobalBounds().intersects(enemyHolder[i].getGlobalBounds())){
						returnObjs[j]->hitEnemy();
						playerPoint++;
						enemyHolder[i].getHit(1);
						hitSound.play();
					}
				}
			} */
			std::vector<Bullet>::iterator itBullet = playerBulletHolder.begin();
			std::vector<Enemy>::iterator itEnemies = enemyHolder.begin();
			while (itBullet != playerBulletHolder.end())
			{
				if(itBullet->isBroken() == false){
					itEnemies = enemyHolder.begin();
					while (itEnemies != enemyHolder.end()) {
						if (itBullet->getGlobalBounds().intersects(itEnemies->getGlobalBounds())) {
							if(itEnemies->isKilled() == false && created){
								if(!BULLET_INVINCIBLE) itBullet->hitEnemy();
								playerPoint++;
								itEnemies->getHit(1);
								hitSound.play();
							}
						}
						itEnemies++;


					}
				}
				itBullet++;

			}



			//Judge collision between bullets
			for(int i = 0; i < static_cast<int>(playerBulletHolder.size()); ++i){
				Vector2f _xy = playerBulletHolder[i].getXY();
				vector<Bullet *> returnObjs = enemyBulletTree.GetObjectsAt(_xy.x, _xy.y);
				for(int j = 0; j < static_cast<int>(returnObjs.size()); ++j){
					if(playerBulletHolder[i].getGlobalBounds().intersects(returnObjs[j]->getGlobalBounds())){
						returnObjs[j]->hitEnemy();
						hitSound.play();
					}
				}
			}

			for(int i = 0; i < static_cast<int>(playerShieldHolder.size()); ++i){
				Vector2f _xy = playerShieldHolder[i].getXY();
				vector<Bullet *> returnObjs = enemyBulletTree.GetObjectsAt(_xy.x, _xy.y);
				for(int j = 0; j < static_cast<int>(returnObjs.size()); ++j){
					if(playerShieldHolder[i].getGlobalBounds().intersects(returnObjs[j]->getGlobalBounds())){
						returnObjs[j]->hitEnemy();
						hitSound.play();
					}
				}
			}

			/* std::vector<Bullet>::iterator itB1 = playerBulletHolder.begin();
			std::vector<Bullet>::iterator itB2 = enemyBulletHolder->begin();
			while (itB1 != playerBulletHolder.end()){
				if(itB1->isBroken() == false){
					itB2 = enemyBulletHolder->begin();
					while(itB2 != enemyBulletHolder->end()){
						if(itB2->isBroken() == false){
							if(itB1->getGlobalBounds().intersects(itB2->getGlobalBounds())){
							// std::cout << "Bullet!\n";
							//itB1->hitEnemy();
							itB2->hitEnemy();
							hitSound.play();
							}
						}
						itB2++;
					}
				}
				itB1++;
			} */

			std::vector<Bullet>::iterator itB3 = playerShieldHolder.begin();
			std::vector<Bullet>::iterator itB4 = enemyBulletHolder->begin();
			while (itB3 != playerShieldHolder.end()){
				itB4 = enemyBulletHolder->begin();
				Vector2f _xy = itB3->getXY();
				vector<Bullet *> returnObjs = enemyBulletTree.GetObjectsAt(_xy.x, _xy.y);
				for(int i = 0; i < static_cast<int>(returnObjs.size()); i++){
					if(itB4->getGlobalBounds().intersects(returnObjs[i]->getGlobalBounds())){
						returnObjs[i]->hitEnemy();

					}
				}
				itB3++;
			}

			//Judge if enemies' bullets hit player
			// std::vector<Bullet>::iterator itBulletE = enemyBulletHolder->begin();
			vector<Bullet *> returnObjs2 = enemyBulletTree.GetObjectsAt(player->getXY().x, player->getXY().y);
			for(int i = 0; i < static_cast<int>(returnObjs2.size()); i++){
				if(returnObjs2[i]->isBroken() == false){
					if(returnObjs2[i]->getGlobalBounds().intersects(player->getGlobalBounds())){
						if(std::chrono::steady_clock::now() - playerLastHitTime > std::chrono::milliseconds(1500)){
							playerLastHitTime = std::chrono::steady_clock::now();
							returnObjs2[i]->hitEnemy();
							if(PLAYER_NOT_INVINCIBLE) player->getHit(1);
							hitSound.play();
						}
					}
				}
			}
			/* while (itBulletE != enemyBulletHolder->end())
			{
				if(itBulletE->isBroken() == false){
					if(itBulletE->getGlobalBounds().intersects(player->getGlobalBounds())){
						if(std::chrono::steady_clock::now() - playerLastHitTime > std::chrono::milliseconds(1500)){
							playerLastHitTime = std::chrono::steady_clock::now();
							itBulletE->hitEnemy();
							player->getHit(1);
							hitSound.play();
						}
					}
				}
				itBulletE++;
			} */

			std::vector<Enemy>::iterator itEnemy = enemyHolder.begin();
			while(enemyStarted && player->isShieldOn() == false && itEnemy != enemyHolder.end()){
				if(itEnemy->getGlobalBounds().intersects(player->getGlobalBounds())){
					if(PLAYER_NOT_INVINCIBLE){
						itEnemy->getHit(1);
						player->getHit(1);
						hitSound.play();
					}
				}
				itEnemy++;
			}

			itEnemy = enemyHolder.begin();
			while(enemyStarted && player->isShieldOn() && itEnemy != enemyHolder.end()){
				if(itEnemy->getGlobalBounds().intersects(player->getShieldBounds())){
					if(itEnemy->isKilled() == false) hitSound.play();
					itEnemy->getHit(1);
				}
				itEnemy++;
			}

			//hit Shield

			Vector2f _xy = player->getXY();
			vector<Bullet *> returnObjs = enemyBulletTree.GetObjectsAt(_xy.x, _xy.y);
			for(int i = 0; i < static_cast<int>(returnObjs.size()); i++){
				if(returnObjs[i]->isBroken() == false){
					if(player->isShieldOn() == true && returnObjs[i]->getGlobalBounds().intersects(player->getShieldBounds())){
						returnObjs[i]->hitEnemy();
						hitShieldSound.play();
					}
				}
			}




			//update bullets
			// VertexArray bulletVertices(Quads);
			for (std::vector<Bullet>::iterator it = playerBulletHolder.begin(); it < playerBulletHolder.end(); it++){
				it->update();
			}

			for (std::vector<Bullet>::iterator it = enemyBulletHolder->begin(); it < enemyBulletHolder->end(); it++){
				it->update();
			}

			for (std::vector<Bullet>::iterator it = playerShieldHolder.begin(); it < playerShieldHolder.end(); it++){
				it->update();
			}

			//update enemies
			std::vector<Enemy>::iterator it2 = enemyHolder.begin();
			while (it2 != enemyHolder.end()){
				if (!it2->isAlive()) {
					it2 = enemyHolder.erase(it2);
					// std::cout << "erased." << std::endl;
				}
				else{
					it2->update();
					it2++;
				}
			}

			if(!KEEP_GAME && created && enemyHolder.size() <= 0){
				win = true;
				GAMESTATE = GAMEOVER;
			}

			if(playerWork){
				player->update();
			}

			enemyBulletTree.clear();


			//Bullet out of bound logic
			Vector2f bPosition;
			std::vector<Bullet>::iterator itB = playerBulletHolder.begin();
			while (itB != playerBulletHolder.end())
			{
				bPosition = itB->getXY();
				if (bPosition.x < 0 || bPosition.x > window->getSize().x || bPosition.y < 0 || bPosition.y > window->getSize().y) {
					itB = playerBulletHolder.erase(itB);
				}else{
					itB++;
				}
			}

			std::vector<Bullet>::iterator itBE = enemyBulletHolder->begin();
			while (itBE != enemyBulletHolder->end())
			{
				bPosition = itBE->getXY();
				if (bPosition.x < 0 || bPosition.x > window->getSize().x || bPosition.y < 0 || bPosition.y > window->getSize().y) {
					itBE = enemyBulletHolder->erase(itBE);
				}else{
					itBE++;
				}
			}

			std::vector<Bullet>::iterator itPS = playerShieldHolder.begin();
			while (itPS != playerShieldHolder.end())
			{
				bPosition = itPS->getXY();
				if (bPosition.x < 0 || bPosition.x > window->getSize().x || bPosition.y < 0 || bPosition.y > window->getSize().y) {
					itPS = playerShieldHolder.erase(itPS);
				}
				else
					itPS++;
			}



			//TEXT
			int objectCnt = 0;
			objectCnt = 1 + playerBulletHolder.size() + enemyBulletHolder->size() + enemyHolder.size();
			objectNumTxt.setString("Object Cnt: " + std::to_string(objectCnt));

			lifeCountTxt.setString("Life: "+std::to_string(player->getLife()));
			lifeCountTxt.setPosition(window->getSize().x-200,window->getSize().y-200);

			pointTxt.setString("Point: "+std::to_string(playerPoint));
			pointTxt.setPosition(window->getSize().x-200,window->getSize().y-150);

			shieldCountTxt.setString("Shield Count: "+std::to_string(playerShieldCount));
			shieldCountTxt.setPosition(window->getSize().x-200,window->getSize().y-100);

			int superShieldWaitTime = std::chrono::duration_cast<std::chrono::seconds>(superShieldInterval - (std::chrono::steady_clock::now() - lastSuperShieldT)).count();
			if(superShieldWaitTime < 0){
				superShieldRechargeTimeTxt.setString("S-Shield: Ready");
			}else{
				superShieldRechargeTimeTxt.setString("Next S-Shield in: " + std::to_string(superShieldWaitTime));
			}

/* 			VertexArray quad(Quads, 4);
			quad[0].position = Vector2f(500,500);
			quad[0].color = Color::White;
			quad[1].position = Vector2f(600,500);
			quad[1].color = Color::White;
			quad[2].position = Vector2f(600,600);
			quad[2].color = Color::White;
			quad[3].position = Vector2f(500,600);
			quad[3].color = Color::White;
			window->draw(quad);
			RectangleShape spe;
			spe.setSize(Vector2f(100, 100));
			spe.setOrigin(50,50);
			spe.setFillColor(Color::White);
			spe.setPosition(500,500);
			window->draw(spe);
 */


			// window->draw(bulletVertices);
			// bulletVertices.clear();
			window->draw(lifeCountTxt);
			window->draw(pointTxt);
			window->draw(shieldCountTxt);
			window->draw(superShieldRechargeTimeTxt);
			window->draw(objectNumTxt);
			if(player->isAlive() == false){
				window->clear();
				GAMESTATE = GAMEOVER;
			}
		}


		if(GAMESTATE == GAMEOVER){
			if(win){
				ggTxt.setString("You won. GG!");


			}else{
				ggTxt.setString("Game Over. GG!");
			}

			if(!resultSoundPlayed){
				if(playerPoint >= 25){
					winSound.play();
				}else{
					loseSound.play();
				}
				resultSoundPlayed = true;
			}

			origin_x = ggTxt.getLocalBounds().width/2;
			origin_y = ggTxt.getLocalBounds().height/2;
			ggTxt.setOrigin(origin_x,origin_y);
			ggTxt.setPosition(window->getSize().x/2,window->getSize().y/2);
			window->draw(ggTxt);

			pointTxt.setOrigin(pointTxt.getGlobalBounds().width/2,pointTxt.getGlobalBounds().width/2);
			pointTxt.setPosition(window->getSize().x/2,window->getSize().y/2+150);
			window->draw(pointTxt);
		}

		window->display();
	}

	return 0;
}
