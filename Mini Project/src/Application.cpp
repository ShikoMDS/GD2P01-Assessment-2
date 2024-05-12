#include "Application.h"

Application::Application() : MUser(sf::Vector2f(100, 100)), MCurrentBehaviour()
{
}

Application::~Application() = default;

void Application::run()
{
	drawWindow();
}

void Application::drawWindow()
{
	//----------Window setup----------//
	sf::RenderWindow Window(sf::VideoMode(1600, 900), "SHIKO LOSING HIS FUCKING MIND IT'S SO OVER GG",
	                        sf::Style::Close);

	auto Image = sf::Image{};
	if (!Image.loadFromFile("resources/images/herta kuru.jpg"))
	{
		// Error handling
	}

	Window.setIcon(Image.getSize().x, Image.getSize().y, Image.getPixelsPtr());
	//----------Window setup----------//

	sf::Clock Clock;

	//----------Running window----------//
	while (Window.isOpen())
	{
		// Handle input events
		handleInput(Window);

		// Calculate delta time
		const float DeltaTime = Clock.restart().asSeconds();

		// Handle user input for the user character
		MUser.handleInput(Window);
		MUser.update(DeltaTime); // Update user character's position

		Window.clear();
		// Draw user character
		MUser.draw(Window);

		// Draw AI entities
		for (auto& Ai : MAiEntities)
		{
			Ai.draw(Window);
		}

		Window.display();
	}
	//----------Running window----------//
}


void Application::handleInput(sf::RenderWindow& Window)
{
	sf::Event Event{};
	while (Window.pollEvent(Event))
	{
		switch (Event.type)
		{
		case sf::Event::Closed:
			Window.close(); // Close the window if the close button is clicked
			break;
		case sf::Event::MouseButtonPressed:
			if (Event.mouseButton.button == sf::Mouse::Left)
			{
				// Load texture for AI entity
				sf::Texture AiTexture;
				if (!AiTexture.loadFromFile("resources/images/herta kuru.jpg"))
				{
					// Error handling
				}

				// Place new AI entity with current behavior at mouse position
				AI NewAi(sf::Vector2f(Event.mouseButton.x, Event.mouseButton.y));
				// Adjust the position to the center of the sprite
				NewAi.setPosition(sf::Vector2f(Event.mouseButton.x - AiTexture.getSize().x / 2,
				                               Event.mouseButton.y - AiTexture.getSize().y / 2));
				NewAi.setTexture(AiTexture); // Set texture for AI entity
				NewAi.setBehaviour(MCurrentBehaviour);
				MAiEntities.push_back(NewAi);
			}
			else if (Event.mouseButton.button == sf::Mouse::Right)
			{
				// Remove AI entity at mouse position
				sf::Vector2f MousePos = Window.mapPixelToCoords(sf::Vector2i(Event.mouseButton.x, Event.mouseButton.y));

				// Use std::remove_if to move the elements to be removed to the end of the vector
				std::erase_if(MAiEntities,
				              [&](const AI& Ai) { return Ai.getGlobalBounds().contains(MousePos); });
			}
			break;
		default:
			break;
		}
	}
}
