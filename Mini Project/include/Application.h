#pragma once

#include <SFML/Graphics.hpp>
#include "User.h"
#include "AI.h"

class Application
{
public:
	Application();
	~Application();

	void run();

private:
	void drawWindow();
	void handleInput(sf::RenderWindow& Window);

	User MUser;

	std::vector<AI> MAiEntities;
	Behaviour MCurrentBehaviour;
};
