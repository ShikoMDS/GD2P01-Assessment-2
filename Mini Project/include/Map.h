// Currently not in use
// Fix later

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Map
{
public:
	Map();
	~Map();

	void loadFromFile(const std::string& Filename, sf::RenderWindow& Window) const;

	static void unloadMap();

private:
	void drawObstacles(sf::RenderWindow& Window, const std::vector<std::vector<char>>& Map) const;

	const float TileSize = 32.0f;
};
