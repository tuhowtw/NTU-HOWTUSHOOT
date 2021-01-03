// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <SFML/Graphics.hpp>
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

int main()
{
	//for controlling process
	bool enemyWork = true;
	bool playerWork = true;


	int GAMESTATE = GAMEMENU;
	bool win = false;

	int windowWidth = 1920;
	int windowHeight = 1080;

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
	SoundBuffer b1, b2, b3, b4, b5, b6, b7, b8;
	Sound shootSound, hitSound, winSound, loseSound, shieldSound, hitShieldSound, startSound, enemySound;
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
	const int CHARGE_MODE = 0, MACHINE_GUN_MODE = 1;
	int playerShootingMode = CHARGE_MODE;
	int playerShootInterval = 10;

	std::chrono::steady_clock::time_point lastSuperShieldT;
	std::chrono::seconds superShieldInterval(10);

	std::vector<Bullet> *enemyBulletHolder = new std::vector<Bullet>;
	int playerPoint = 0;

	std::chrono::steady_clock::time_point playerLastHitTime;


	Texture *enemyTexture = new Texture();
	enemyTexture->loadFromFile("Textures/enemy.png");
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

	Vector2f enemyXY;
	Vector2f enemyStartPoint;
	int enemyInterval = 40;
	enemyStartPoint.x = window->getSize().x/2-2*enemyInterval;
	enemyStartPoint.y = window->getSize().y/2-2*enemyInterval;
	std::vector<Enemy> enemyHolder;
	std::chrono::steady_clock::time_point lastSpawnT;
/*
	bool lineStarting = false;
	Vector2f lineTargetXY;
	Enemy* lineMembers[10]; */
	// int lineMemberStartLatency = 0;
	std::chrono::steady_clock::time_point lastLineStartT;

	while (window->isOpen())
	{
		Event event;
		while (window->pollEvent(event)) {
			if (event.type == Event::Closed) window->close();
		}
		if (event.type == sf::Event::Resized)
		{
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
			if(!created){
				if(!spawnStart){
					if(std::chrono::steady_clock::now() - lastCreatedTime > std::chrono::milliseconds(5)){
						if(i < 10){
							if(j < 10){
								enemyXY.x = enemyStartPoint.x + i * enemyInterval;
								enemyXY.y = enemyStartPoint.y + j * enemyInterval;
								enemyHolder.push_back(Enemy(enemyXY, window, player, enemyTexture, 1, explosionTexture, enemyBulletHolder, 3));
								lastCreatedTime = std::chrono::steady_clock::now();
								enemySound.play();
								count++;
								j++;
							}
							if(j == 10){
								j = 0;
								i++;
							}
						}
					}
					if(count >= 100){
						created = true;
						enemyCreatedT = std::chrono::steady_clock::now();
					}
				}
			}

			if(!enemyWork){
				created = false;
			}
			//created = false;
			//wait for 2 sec then enemies move
			auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - enemyCreatedT);
			if(elapsed_seconds > std::chrono::seconds(0) && !enemyStarted && created){
				enemyStarted = true;
				std::vector<Enemy>::iterator itStart = enemyHolder.begin();
				while (itStart != enemyHolder.end()){
					itStart->startMoving();
					itStart++;
				}
			}

			if(created && std::chrono::steady_clock::now() - lastCreatedTime > std::chrono::milliseconds(3000 + std::rand()% 6 * 250)){
				lastCreatedTime = std::chrono::steady_clock::now();
				Vector2f enemyXY, playerXY;
				enemyXY.x = std::rand()%window->getSize().x;
				enemyXY.y = std::rand()%window->getSize().y;
				playerXY = player->getXY();
				int startLatency = 0;
				for(int i = 0; i < 10; i++){
					startLatency += 150;
					enemyHolder.push_back(Enemy(enemyXY, window, player, enemyTexture, 1, explosionTexture, enemyBulletHolder, 2));
					enemyHolder[enemyHolder.size() - 1].startAfterMiliSec(startLatency, playerXY);
					// lineMembers[i] = &enemyHolder[enemyHolder.size() - 1];
				}
				enemySound.play();
			}

			/* if(lineStarting){
				if(std::chrono::steady_clock::now() - lastLineStartT > std::chrono::milliseconds(150)){
					if(lineMemeberIndex < 10){
						//start member
						lineMembers[lineMemeberIndex++]->startMoving(lineTargetXY);
						lastLineStartT = std::chrono::steady_clock::now();
					}else{
						lineStarting = false;
					}
				}
			} */


			// if(enemyStarted && !lineCreated){
			// 	for(int i = 0; i < 10; i++){
			// 		enemyHolder.push_back(Enemy(enemyXY, window, player, enemyTexture, 1, explosionTexture, enemyBulletHolder, 2));
			// 		lineMembers[i] = enemyHolder
			// 	}
			// 	lineCreated = true;
			// }

			// if()



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
				playerShootingMode = MACHINE_GUN_MODE;
			}

			//trigger
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if(playerShootingMode == CHARGE_MODE){
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

						playerBulletHolder.push_back(Bullet(player->getSprite(), mouseXY, window, "white", 3 * playerBulletPower, 3 * playerBulletPower, angle));
						//lastShotTime = std::clock();
					}else if(std::chrono::steady_clock::now() - bulletPowerT > std::chrono::milliseconds(100)){
						bulletPowerT = std::chrono::steady_clock::now();
						if(playerBulletPower < 10){
							playerBulletPower += 1;
						}
						playerBulletHolder[playerBulletHolder.size() - 1].setSize(3 * playerBulletPower, 3 * playerBulletPower);
					}

					mouseLPressed = true;
				}else if(playerShootingMode == MACHINE_GUN_MODE){
					Vector2f mouseXY;
					Vector2i iMouseXY = Mouse::getPosition(*window);
					mouseXY.x = iMouseXY.x;
					mouseXY.y = iMouseXY.y;
					if(std::chrono::steady_clock::now() - playerLastShootTime > std::chrono::milliseconds(playerShootInterval)){
						playerBulletHolder.push_back(Bullet(player->getSprite(), mouseXY, window, "white", 3 * playerBulletPower, 3 * playerBulletPower, -1));
						playerBulletHolder[playerBulletHolder.size()-1].start(mouseXY, 3 * playerBulletPower, 3 * playerBulletPower, playerBulletPower);
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
					shootSound.play();

				}
				mouseLPressed = false;
				playerBulletPower = 1;
			}

			if(Mouse::isButtonPressed(Mouse::Right) == false){
				mouseRPressed = false;
			}



			//judge if bullets hit enemies
			std::vector<Bullet>::iterator itBullet = playerBulletHolder.begin();
			std::vector<Enemy>::iterator itEnemies = enemyHolder.begin();
			while (itBullet != playerBulletHolder.end())
			{
				if(itBullet->isBroken() == false){
					itEnemies = enemyHolder.begin();
					while (itEnemies != enemyHolder.end()) {
						if (itBullet->getGlobalBounds().intersects(itEnemies->getGlobalBounds())) {
							if(itEnemies->isKilled() == false && created){
								itBullet->hitEnemy();
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

			//Judge collision betwwen bullets
			// std::vector<Bullet>::iterator itB1 = playerBulletHolder.begin();
			// std::vector<Bullet>::iterator itB2 = enemyBulletHolder->begin();
			// while (itB1 != playerBulletHolder.end()){
			// 	if(itB1->isBroken() == false){
			// 		itB2 = enemyBulletHolder->begin();
			// 		while(itB2 != enemyBulletHolder->end()){
			// 			if(itB2->isBroken() == false){
			// 				if(itB1->getGlobalBounds().intersects(itB2->getGlobalBounds())){
			// 				std::cout << "Bullet!\n";
			// 				//itB1->hitEnemy();
			// 				itB2->hitEnemy();
			// 				hitSound.play();
			// 				}
			// 			}
			// 			itB2++;
			// 		}
			// 	}
			// 	itB1++;
			// }

			std::vector<Bullet>::iterator itB3 = playerShieldHolder.begin();
			std::vector<Bullet>::iterator itB4 = enemyBulletHolder->begin();
			while (itB3 != playerShieldHolder.end()){
				itB4 = enemyBulletHolder->begin();
				while(itB4 != enemyBulletHolder->end()){
					if(itB3->isBroken() == false && itB4->isBroken()==false){
						if(itB3->getGlobalBounds().intersects(itB4->getGlobalBounds())){
							// std::cout << "Bullet!\n";
							//itB3->hitEnemy();
							itB4->hitEnemy();
						}

					}
					itB4++;
				}
				itB3++;
			}

			//Judge if enemies' bullets hit player
			std::vector<Bullet>::iterator itBulletE = enemyBulletHolder->begin();
			while (itBulletE != enemyBulletHolder->end())
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
			}

			std::vector<Enemy>::iterator itEnemy = enemyHolder.begin();
			while(player->isShieldOn() == false && itEnemy != enemyHolder.end()){
				if(itEnemy->getGlobalBounds().intersects(player->getGlobalBounds())){
					itEnemy->getHit(1);
					player->getHit(1);
					hitSound.play();
				}
				itEnemy++;
			}

			//hit Shield
			itBulletE = enemyBulletHolder->begin();
			while (itBulletE != enemyBulletHolder->end()){
				if(itBulletE->isBroken() == false){
					if(player->isShieldOn() == true && itBulletE->getGlobalBounds().intersects(player->getShieldBounds())){
						itBulletE->hitEnemy();
						hitShieldSound.play();
					}
				}
				itBulletE++;
			}

			itEnemy = enemyHolder.begin();
			while(player->isShieldOn() && itEnemy != enemyHolder.end()){
				if(itEnemy->getGlobalBounds().intersects(player->getShieldBounds())){
					if(itEnemy->isKilled() == false) hitSound.play();
					itEnemy->getHit(1);
				}
				itEnemy++;
			}


			//update bullets
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

			if(created && enemyHolder.size() <= 0){
				win = true;
				GAMESTATE = GAMEOVER;
			}

			if(playerWork){
				player->update();
			}


			//Bullet out of bound logic
			Vector2f bPosition;
			std::vector<Bullet>::iterator itB = playerBulletHolder.begin();
			while (itB != playerBulletHolder.end())
			{
				bPosition = itB->getXY();
				if (bPosition.x < 0 || bPosition.x > window->getSize().x || bPosition.y < 0 || bPosition.y > window->getSize().y) {
					itB = playerBulletHolder.erase(itB);
				}
				else
					itB++;
			}

			std::vector<Bullet>::iterator itBE = enemyBulletHolder->begin();
			while (itBE != enemyBulletHolder->end())
			{
				bPosition = itBE->getXY();
				if (bPosition.x < 0 || bPosition.x > window->getSize().x || bPosition.y < 0 || bPosition.y > window->getSize().y) {
					itBE = enemyBulletHolder->erase(itBE);
				}
				else
					itBE++;
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
