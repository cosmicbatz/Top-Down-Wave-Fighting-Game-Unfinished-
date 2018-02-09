#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include "SFML\Audio.hpp"
#include <cstdlib>

// Screen width and height
int screenWidth = 1080;
int screenHeight = 720;

// Window for the game
sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Dungeon Fighters");

