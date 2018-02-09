#pragma once
#include <iostream>
#include "gameWindow.h"


// types of errors
enum errorTypes { logicError = 0, textureError, spriteError, soundError, musicError, fstreamError };
// error function
void error(errorTypes errorType, std::string addInfo, int lineNumber) {
	switch (errorType) {
	case logicError:
		std::cerr << "Logic error! Terminating program!" << std::endl;
		exit(1);
		break;
	case textureError:
		std::cerr << "Could not load texture!" << std::endl;
		exit(1);
		break;
	case spriteError:
		std::cerr << "Could not load sprite!" << std::endl;
		break;
	case soundError:
		std::cerr << "Could not load sound!" << std::endl;
		break;
	case musicError:
		std::cerr << "Could not load music!" << std::endl;
		break;
	case fstreamError:
		std::cerr << "Could not load file!" << std::endl;
		break;
	}
}

class gameLevels {
protected:
	enum enemyType { slime = 0, };
	// level one
	sf::Texture levelOneTexture;
	sf::Sprite levelOneSprite;

	// level two
	sf::Texture levelTwoTexture;
	sf::Sprite levelTwoSprite;

	// level three
	sf::Texture levelThreeTexture;
	sf::Sprite levelThreeSprite;
public:
	gameLevels() {
		// level one
		if (!levelOneTexture.loadFromFile("floor.png")) {
			error(textureError, 0, 0);
		}
		levelOneTexture.setRepeated(true);
		levelOneSprite.setTexture(levelOneTexture);
		levelOneSprite.setTextureRect(sf::IntRect(2000, 1000, 2000, 1000));
	}
};
