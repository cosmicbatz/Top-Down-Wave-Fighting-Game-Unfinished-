// dungeonFighters.cpp : Defines the entry point for the console application.
// Programmer: Liam C.

#include "stdafx.h"
#include "levels.h"

int main();

// class to create player objects and easily set their attributes
class player {
public:
	player() {

	}

	float walkSpeed = 0.2;
	float runSpeed = 0.5;
	//float walkSpeed = 0.05;
	//float runSpeed = 0.1;
	int playerHealth = 5;
	int playerAttackDmg;
	bool playerAttacked = false;
	//sfml variables
	sf::Texture characterTexture;
	sf::Sprite character;
	sf::Texture playerAttackTexture;
	sf::Sprite playerAttack;
	sf::Texture healthTexture[6];
	sf::Sprite healthSprite[6];
};

// class to create enemy objects and easily set their attributes
class enemy {
public:
	int moveSpeed;
	int spawnX = rand() % 700 + 1;
	int spawnY = rand() % 700 + 1;
	int enemyHealth = 1;
	int enemyAttackDmg;
	float enemySpeed = 0.009;
	bool alive = true;
	// sfml variables
	sf::Texture enemyTexture;
	sf::Sprite enemySprite;
};

// class to create music objects and easily set their attributes
class music {
public:
	sf::Music song;
	void setSong(std::string songFileName) {
		if (!song.openFromFile(songFileName)) {
			error(soundError, 0, 0);
		}
	}
	void playAudio(bool play) {
		if (play) {
			// play music
			song.play();
		}
		else {
			// stop music
			song.stop();
		}
	}
};

class mainMenu {
private:
	// sfml variables
	// textures
	sf::Texture menuBackgroundTexture;
	// sprites
	sf::Sprite menuBackground;

	// variables
	// whether to exit the menu and begin the game
	bool startGame = false;

	// main menu song object
	music mainMenuSong;

	// event variable
	sf::Event eV;

	// fonts
	sf::Font arial;

	// texts
	sf::Text mainMenuText;
public:
	mainMenu() {
		// init textures
		if (!menuBackgroundTexture.loadFromFile("mainMenu.jpg")) {
			error(textureError, "", NULL);
		}
		// init sprites
		menuBackground.setTexture(menuBackgroundTexture);
		//  set main menu song
		mainMenuSong.setSong("mainMenuMusic.wav");
		mainMenuSong.song.setVolume(50);
		mainMenuSong.song.setLoop(true);
		mainMenuSong.playAudio(true);
		// set font and text
		if (!arial.loadFromFile("arial.ttf")) {
			error(logicError, 0, 0);
		}
		//set font to text
		mainMenuText.setFont(arial);
		mainMenuText.setString("- Dungeon Fighters - 1.0.0");
		mainMenuText.setFillColor(sf::Color::Red);
		mainMenuText.setStyle(sf::Text::Bold);
	}

	void mainMenuDraw() {
		window.clear();
		window.draw(menuBackground);
		window.draw(mainMenuText);
		window.display();
	}

	int mainMenuInput() {
		while (window.pollEvent(eV)) {
			switch (eV.type) {
			case sf::Event::Closed:
				// stop playing song
				mainMenuSong.playAudio(false);
				// close game window
				window.close();
				// stop program
				exit(0);
				break;
			case sf::Event::KeyPressed:
				mainMenuSong.playAudio(false);
				startGame = true;
				break;
			}
		}// end of event loop
	}

	void mainMenuLogic() {

	}

	void mainMenuLoop() {
		// while the user hasn't exited from the main menu
		while (!startGame) {
			// draw things
			mainMenuDraw();
			// events and user input
			mainMenuInput();
			// game logic
			mainMenuLogic();
		}
	}
};

class playGame : public gameLevels {
private:
	// bools
	// whether the game is over
	bool gameOver = false;
	// whether the game is paused
	bool isGamePaused = false;
	// whether there is a cutscene
	bool cutScene = false;
	// whether player one is running
	bool playerOneRunning = false;
	// whether player two is running
	bool playerTwoRunning = false;
	// whether the players want to play again
	bool playAgain = false;
	// string answer to playing again
	std::string answer;
	// Direction players are facing
	enum dir { UP = 0, DOWN, LEFT, RIGHT };
	dir playerOneDir;
	dir playerTwoDir;
	// player objects
	player playerOne;
	player playerTwo;
	// enemy objects
	enemy slime[10];
	// music objects
	music gameMusic;
	music attackSound;
	music slimeDeathSound;
	// number of enemies killed
	int score = 0;
	// to move the positioning of the players attacks
	int playerOneAttackMoveX, playerOneAttackMoveY, playerTwoAttackMoveX, playerTwoAttackMoveY;

public:
	playGame() {
		// initialize sfml variables
		//textures
		//player One texture
		if (!playerOne.characterTexture.loadFromFile("mage_sheet.png")) {
			error(textureError, 0, 0);
		}
		playerOne.character.setTexture(playerOne.characterTexture);
		playerOne.character.setPosition(400, 300);
		playerOne.character.setTextureRect(sf::IntRect(10, 140, 50, 60));
		// player one attack texture
		if (!playerOne.playerAttackTexture.loadFromFile("fireball.bmp")) {
			error(textureError, 0, 0);
		}
		playerOne.playerAttack.setScale(2, 2);
		playerOne.playerAttack.setTexture(playerOne.playerAttackTexture);
		// player two texture
		if (!playerTwo.characterTexture.loadFromFile("knight_sheet.png")) {
			error(textureError, 0, 0);
		}
		playerTwo.character.setTexture(playerTwo.characterTexture);
		playerTwo.character.setPosition(600, 300);
		playerTwo.character.setTextureRect(sf::IntRect(10, 140, 50, 60));
		// player two attack texture
		if (!playerTwo.playerAttackTexture.loadFromFile("sword2.png")) {
			error(textureError, 0, 0);
		}
		playerTwo.playerAttack.setTexture(playerTwo.playerAttackTexture);
		// main music
		gameMusic.setSong("gameMusic.ogg");
		gameMusic.song.setVolume(50);
		gameMusic.song.setLoop(true);
		gameMusic.playAudio(true);
		// health sfml variable initialization 
		// player one health 
		//if (!playerOne.healthTexture[0].loadFromFile("Health_0.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerOne.healthTexture[1].loadFromFile("Health_1.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerOne.healthTexture[2].loadFromFile("Health_2.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerOne.healthTexture[3].loadFromFile("Health_3.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerOne.healthTexture[4].loadFromFile("Health_4.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerOne.healthTexture[5].loadFromFile("Health_5.png")) {
		//	error(textureError, 0, 0);
		//}
		//// player two health
		//if (!playerTwo.healthTexture[0].loadFromFile("verticalHealth_0.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerTwo.healthTexture[1].loadFromFile("verticalHealth_1.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerTwo.healthTexture[2].loadFromFile("verticalHealth_2.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerTwo.healthTexture[3].loadFromFile("verticalHealth_3.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerTwo.healthTexture[4].loadFromFile("verticalHealth_4.png")) {
		//	error(textureError, 0, 0);
		//}
		//if (!playerTwo.healthTexture[5].loadFromFile("verticalHealth_5.png")) {
		//	error(textureError, 0, 0);
		//}
		for (int i = 0; i < 6; i++) { //  set the health textures to their corresponding sprites for both players
			playerOne.healthSprite[i].setTexture(playerOne.healthTexture[i]);
			playerTwo.healthSprite[i].setTexture(playerTwo.healthTexture[i]);
		}
		// init enemy sfml 
		for (int i = 0; i < 10; i++) {
			if (!slime[i].enemyTexture.loadFromFile("slime.png")) {
				error(textureError, 0, 0);
			}
			slime[i].enemySprite.setTexture(slime[i].enemyTexture);
			slime[i].enemySprite.setPosition(slime[i].spawnX, slime[i].spawnY);
			slime[i].enemySprite.setScale(1.5f, 1.5f);
			slime[i].enemySprite.setTextureRect(sf::IntRect(0, 0, 30, 30));
		}

	}
	//function to reset variables

	// function to restart game
	void restart() {
		gameOver = false;
		score = 0;
		playerOne.playerHealth = 5;
		playerTwo.playerHealth = 5;

		main();
	}

	void gameLoop() {
		while (!gameOver) {
			drawGame();
			input();
			logic();
		}
		gameMusic.playAudio(false);
		std::cout << "GAME OVER." << std::endl;
		std::cout << "SCORE THIS ROUND: " << score << std::endl;
		std::cout << "Would you like to play again?" << std::endl;
		std::cin >> answer;
		// if the player wants to play again, restart the game
		if (answer == "y" || answer == "yes" || answer ==  "Y") {
			restart();
		}
		// otherwise, exit the game
		else {
			exit(0);
		}
	}
	void input() {
		// event variable
		sf::Event e;
		// check for events
		while (window.pollEvent(e)) {
			switch (e.type) {
			default:
				break;
				// close window
			case sf::Event::Closed:
				gameMusic.playAudio(false);
				window.close();
				gameOver = true;
				break;
				// press a key
			case sf::Event::KeyPressed:
				if (e.key.code == sf::Keyboard::Escape) {
				//	isGamePaused = true;
				}
				break;
			}// end of switch
		}// end of while loop

		if (!cutScene) {
			// Player one controls
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				// set character's direction enum to current direction
				playerOneDir = UP;
				playerOne.character.setTextureRect(sf::IntRect(10, 10, 50, 60));
				// walking
				if (!playerOneRunning) {
					playerOne.character.move(0, -(playerOne.walkSpeed));
				}
				// running
				else {
					playerOne.character.move(0, -(playerOne.runSpeed));
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				playerOneDir = LEFT;
				playerOne.character.setTextureRect(sf::IntRect(10, 70, 50, 60));
				if (!playerOneRunning) {
					playerOne.character.move(-(playerOne.walkSpeed), 0);
				}
				else {
					playerOne.character.move(-(playerOne.runSpeed), 0);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				playerOneDir = DOWN;
				playerOne.character.setTextureRect(sf::IntRect(10, 140, 50, 60));
				if (!playerOneRunning) {
					playerOne.character.move(0, playerOne.walkSpeed);
				}
				else {
					playerOne.character.move(0, (playerOne.runSpeed));
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				playerOneDir = RIGHT;
				playerOne.character.setTextureRect(sf::IntRect(0, 200, 70, 55));
				if (!playerOneRunning) {
					playerOne.character.move(playerOne.walkSpeed, 0);
				}
				else {
					playerOne.character.move(playerOne.runSpeed, 0);
				}
			}
			// runnning
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				playerOneRunning = true;
			}
			else {
				playerOneRunning = false;
			}
			// attacking
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				// set attack flag to true
				playerOne.playerAttacked = true;
				// attack sprite oriented based on direction of player
				if (playerOneDir == UP) {
					playerOneAttackMoveX = 15;
					playerOneAttackMoveY = -90;
					playerOne.playerAttack.setRotation(0);
				}
				else if (playerOneDir == DOWN) {
					playerOne.playerAttack.setRotation(180);
					playerOneAttackMoveX = 30;
					playerOneAttackMoveY = 150;
				}
				else if (playerOneDir == LEFT) {
					playerOne.playerAttack.setRotation(-90);
					playerOneAttackMoveX = -90;
					playerOneAttackMoveY = 40;
				}
				else if (playerOneDir == RIGHT) {
					playerOne.playerAttack.setRotation(90);
					playerOneAttackMoveX = 145;
					playerOneAttackMoveY = 22;
				}
			}

			// Player two controls
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				playerTwoDir = UP;
				playerTwo.character.setTextureRect(sf::IntRect(10, 10, 50, 60));
				// walking
				if (!playerTwoRunning)
				{
					playerTwo.character.move(0, -(playerTwo.walkSpeed));
				}
				//running
				else {
					playerTwo.character.move(0, -playerTwo.runSpeed);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				playerTwoDir = DOWN;
				playerTwo.character.setTextureRect(sf::IntRect(10, 140, 50, 60));
				if (!playerTwoRunning) {
					playerTwo.character.move(0, playerTwo.walkSpeed);
				}
				else {
					playerTwo.character.move(0, playerTwo.runSpeed);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				playerTwoDir = LEFT;
				playerTwo.character.setTextureRect(sf::IntRect(10, 70, 50, 70));
				if (!playerTwoRunning) {
					playerTwo.character.move(-playerTwo.walkSpeed, 0);
				}
				else {
					playerTwo.character.move(-playerTwo.runSpeed, 0);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				playerTwoDir = RIGHT;
				playerTwo.character.setTextureRect(sf::IntRect(0, 200, 70, 70));
				if (!playerTwoRunning) {
					playerTwo.character.move(playerTwo.walkSpeed, 0);
				}
				else {
					playerTwo.character.move(playerTwo.runSpeed, 0);
				}
			}
			// running
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Insert)) {
				playerTwoRunning = true;
			}
			else {
				playerTwoRunning = false;
			}
			// attacking
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::End)) {
				playerTwo.playerAttacked = true;
				if (playerTwoDir == UP) {
					playerTwo.playerAttack.setRotation(0);
					playerTwoAttackMoveX = 15;
					playerTwoAttackMoveY = -30;
				}
				else if (playerTwoDir == DOWN) {
					playerTwo.playerAttack.setRotation(180);
					playerTwoAttackMoveX = 30;
					playerTwoAttackMoveY = 110;
				}
				else if (playerTwoDir == LEFT) {
					playerTwo.playerAttack.setRotation(-90);
					playerTwoAttackMoveX = -30;
					playerTwoAttackMoveY = 40;
				}
				else if (playerTwoDir == RIGHT) {
					playerTwo.playerAttack.setRotation(90);
					playerTwoAttackMoveX = 85;
					playerTwoAttackMoveY = 25;
				}

			}
		}
	}

	void logic() {
		// MAP BOUNDARIES
		// players
		// player one x
		if (playerOne.character.getPosition().x <= 0) {
			playerOne.character.move(1, 0);
		}
		else if (playerOne.character.getPosition().x >= 1020) {
			playerOne.character.move(-1, 0);
		}
		// player one y
		if (playerOne.character.getPosition().y <= 0) {
			playerOne.character.move(0, 1);
		}
		if (playerOne.character.getPosition().y >= 620) {
			playerOne.character.move(0, -1);
		}
		// player two x
		if (playerTwo.character.getPosition().x <= 0) {
			playerTwo.character.move(1, 0);
		}
		else if (playerTwo.character.getPosition().x >= 1020) {
			playerTwo.character.move(-1, 0);
		}
		// player two y
		if (playerTwo.character.getPosition().y <= 0) {
			playerTwo.character.move(0, 1);
		}
		if (playerTwo.character.getPosition().y >= 620) {
			playerTwo.character.move(0, -1);
		}
		// HITBOXES
		// player hitbox and enemy movement
		for (int i = 0; i < 10; i++) {
			// player one hitbox
			if (slime[i].alive) {
				if (playerOne.character.getPosition().x < slime[i].enemySprite.getPosition().x + 5 && playerOne.character.getPosition().x > slime[i].enemySprite.getPosition().x - 5
					&& playerOne.character.getPosition().y < slime[i].enemySprite.getPosition().y + 5 && playerOne.character.getPosition().y > slime[i].enemySprite.getPosition().y - 5) {
					playerOne.playerHealth -= 1;
					if (playerOne.playerHealth == 0) {
						// game over
						std::cout << "PLAYER ONE DIED." << std::endl;
						gameOver = true;
					}
				}
				// player two hitbox
				if (playerTwo.character.getPosition().x < slime[i].enemySprite.getPosition().x + 5 && playerTwo.character.getPosition().x > slime[i].enemySprite.getPosition().x - 5
					&& playerTwo.character.getPosition().y < slime[i].enemySprite.getPosition().y + 5 && playerTwo.character.getPosition().y > slime[i].enemySprite.getPosition().y - 5) {
					playerTwo.playerHealth -= 1;
					if (playerTwo.playerHealth == 0) {
						// game over
						std::cout << "PLAYER TWO DIED." << std::endl;
						gameOver = true;
					}
				}
			}
			// move enemies towards player one
			for (int i = 0; i < 5; i++) {
				if (playerOne.character.getPosition().x > slime[i].enemySprite.getPosition().x) {
					slime[i].enemySprite.move(slime[i].enemySpeed, 0);
				}
				else if (playerOne.character.getPosition().x < slime[i].enemySprite.getPosition().x) {
					slime[i].enemySprite.move(-slime[i].enemySpeed, 0);
				}
				if (playerOne.character.getPosition().y > slime[i].enemySprite.getPosition().y) {
					slime[i].enemySprite.move(0, slime[i].enemySpeed);
				}
				else if (playerOne.character.getPosition().y < slime[i].enemySprite.getPosition().y) {
					slime[i].enemySprite.move(0, -slime[i].enemySpeed);
				}
			}
			// move enemies towards player two
			for (int i = 5; i < 10; i++) {
				if (playerTwo.character.getPosition().x > slime[i].enemySprite.getPosition().x) {
					slime[i].enemySprite.move(slime[i].enemySpeed, 0);
				}
				else if (playerTwo.character.getPosition().x < slime[i].enemySprite.getPosition().x) {
					slime[i].enemySprite.move(-slime[i].enemySpeed, 0);
				}
				if (playerTwo.character.getPosition().y > slime[i].enemySprite.getPosition().y) {
					slime[i].enemySprite.move(0, slime[i].enemySpeed);
				}
				else if (playerTwo.character.getPosition().y < slime[i].enemySprite.getPosition().y) {
					slime[i].enemySprite.move(0, -slime[i].enemySpeed);
				}
			}
			// Enemy / Attack Hitboxes
			// player one
			if (playerOne.playerAttacked == true) {
				// coordinates depend on direction player is facing
				if (playerOneDir == UP) {
					// if coordinates in relation to the enemy are acceptable, kill the enemy
					if (playerOne.playerAttack.getPosition().x < slime[i].enemySprite.getPosition().x + 20 && playerOne.playerAttack.getPosition().x > slime[i].enemySprite.getPosition().x - 20
						&& slime[i].enemySprite.getPosition().y > playerOne.playerAttack.getPosition().y - 20 && slime[i].enemySprite.getPosition().y < playerOne.playerAttack.getPosition().y + 100) {
						slime[i].alive = false;
						score++;
					}
				}
				if (playerOneDir == DOWN) {
					if (slime[i].enemySprite.getPosition().x < (playerOne.playerAttack.getPosition().x) + 1 && slime[i].enemySprite.getPosition().x >(playerOne.playerAttack.getPosition().x) - 35
						&& slime[i].enemySprite.getPosition().y > playerOne.playerAttack.getPosition().y - 110 && slime[i].enemySprite.getPosition().y < playerOne.playerAttack.getPosition().y + 15) {
						slime[i].alive = false;
						score++;
					}
				}
				if (playerOneDir == LEFT) {
					if (slime[i].enemySprite.getPosition().x < (playerOne.playerAttack.getPosition().x) + 100 && slime[i].enemySprite.getPosition().x >(playerOne.playerAttack.getPosition().x) - 30
						&& slime[i].enemySprite.getPosition().y > playerOne.playerAttack.getPosition().y - 20 && slime[i].enemySprite.getPosition().y < playerOne.playerAttack.getPosition().y + 20) {
						slime[i].alive = false;
						score++;
					}
				}
				if (playerOneDir == RIGHT) {
					if (slime[i].enemySprite.getPosition().x < (playerOne.playerAttack.getPosition().x) - 30 && slime[i].enemySprite.getPosition().x >(playerOne.playerAttack.getPosition().x) + 100
						&& slime[i].enemySprite.getPosition().y > playerOne.playerAttack.getPosition().y - 20 && slime[i].enemySprite.getPosition().y < playerOne.playerAttack.getPosition().y + 20) {
						slime[i].alive = false;
						score++;
					}
				}
			}

			// player two
			if (playerTwoDir == UP) {
				if (playerTwo.playerAttack.getPosition().x < slime[i].enemySprite.getPosition().x + 20 && playerTwo.playerAttack.getPosition().x > slime[i].enemySprite.getPosition().x - 20
					&& slime[i].enemySprite.getPosition().y > playerTwo.playerAttack.getPosition().y - 20 && slime[i].enemySprite.getPosition().y < playerTwo.playerAttack.getPosition().y + 120) {
					slime[i].alive = false;
					score++;
				}
			}
			if (playerTwoDir == DOWN) {
				if (slime[i].enemySprite.getPosition().x < (playerTwo.playerAttack.getPosition().x) + 1 && slime[i].enemySprite.getPosition().x >(playerTwo.playerAttack.getPosition().x) - 35
					&& slime[i].enemySprite.getPosition().y > playerTwo.playerAttack.getPosition().y - 110 && slime[i].enemySprite.getPosition().y < playerTwo.playerAttack.getPosition().y + 5) {
					slime[i].alive = false;
					score++;
				}
			}
			if (playerTwoDir == LEFT) {
				if (slime[i].enemySprite.getPosition().x < (playerTwo.playerAttack.getPosition().x) + 100 && slime[i].enemySprite.getPosition().x >(playerTwo.playerAttack.getPosition().x) - 30
					&& slime[i].enemySprite.getPosition().y > playerTwo.playerAttack.getPosition().y - 1 && slime[i].enemySprite.getPosition().y < playerTwo.playerAttack.getPosition().y + 20) {
					slime[i].alive = false;
					score++;
				}
			}
			if (playerTwoDir == RIGHT) {
				if (slime[i].enemySprite.getPosition().x < (playerTwo.playerAttack.getPosition().x) - 30 && slime[i].enemySprite.getPosition().x >(playerTwo.playerAttack.getPosition().x) + 100
					&& slime[i].enemySprite.getPosition().y > playerTwo.playerAttack.getPosition().y - 20 && slime[i].enemySprite.getPosition().y < playerTwo.playerAttack.getPosition().y + 20) {
					slime[i].alive = false;
					score++;
				}
			}
			// prevent enemies from stacking


		}// end of for loop for player hit box and enemy movement
	}

	void drawGame() {
		window.clear();
		// draw floor
		window.draw(levelOneSprite);
		// draw player attacks if they press the attack flag is true
		if (playerOne.playerAttacked == true) {
			// reset attack flag after attack code runs
			playerOne.playerAttacked = false;
			playerOne.playerAttack.setPosition((playerOne.character.getPosition().x) + playerOneAttackMoveX, (playerOne.character.getPosition().y) + playerOneAttackMoveY);
			window.draw(playerOne.playerAttack);
		}
		if (playerTwo.playerAttacked == true) {
			playerTwo.playerAttacked = false;
			playerTwo.playerAttack.setPosition((playerTwo.character.getPosition().x) + playerTwoAttackMoveX, (playerTwo.character.getPosition().y) + playerTwoAttackMoveY);
			window.draw(playerTwo.playerAttack);
		}
		// draw players' health using amount as index value
		//	window.draw(playerOne.healthSprite[playerOne.playerHealth]);
		//	window.draw(playerOne.healthSprite[playerTwo.playerHealth]);
		// draw players 
		window.draw(playerOne.character);
		window.draw(playerTwo.character);
		// draw enemies
		for (int i = 0; i < 10; i++) {
			if (slime[i].alive == true) {
				window.draw(slime[i].enemySprite);
			}
		}
		window.display();
	}
};

int main()
{
	srand(time(NULL));
	mainMenu openMenu;
	openMenu.mainMenuLoop();
	playGame start;
	start.gameLoop();

	return 0;
}

