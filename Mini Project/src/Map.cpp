// Currently not in use.
// Fix later

#include "Map.h"

Map::Map() = default;

Map::~Map()
{
	unloadMap();
}

void Map::loadFromFile(const std::string& Filename, sf::RenderWindow& Window) const
{
	std::ifstream File(Filename);
	if (!File.is_open())
	{
		std::cerr << "Failed to open map file: " << Filename << std::endl;
		return;
	}

	std::vector<std::vector<char>> Map;
	std::string Line;
	while (std::getline(File, Line))
	{
		std::vector<char> Row(Line.begin(), Line.end());
		Map.push_back(Row);
	}

	File.close();

	// Draw obstacles from map
	drawObstacles(Window, Map);
}

void Map::unloadMap()
{
}

void Map::drawObstacles(sf::RenderWindow& Window, const std::vector<std::vector<char>>& Map) const
{
	std::cout << "Drawing obstacles..." << std::endl;

	const sf::Color ObstacleColor = sf::Color::Red;

	sf::RectangleShape Obstacle(sf::Vector2f(TileSize, TileSize));
	Obstacle.setFillColor(ObstacleColor);

	for (size_t I = 0; I < Map.size(); ++I)
	{
		for (size_t J = 0; J < Map[I].size(); ++J)
		{
			if (Map[I][J] == '#')
			{
				Obstacle.setPosition(J * TileSize, I * TileSize);
				Window.draw(Obstacle);
				std::cout << "Obstacle drawn at position: (" << J << ", " << I << ")" << std::endl;
			}
		}
	}
}
