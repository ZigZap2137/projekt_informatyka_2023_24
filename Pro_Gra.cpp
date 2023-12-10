#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>


int main()
{
	sf::VideoMode vm(800, 600);
	sf::RenderWindow window(vm, "SFML animation", sf::Style::Default);
	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
					window.close();
					break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;

			}
		}		
	}

	//update
	//10.12.23 okno programu

	//render
	window.clear(sf::Color(200,200,200,200));

	window.display();
}