#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	void iniVar();//zmiennych
	void iniWin();//okna
public:
	//konstruktory
	Game();
	virtual ~Game();

	//dostepy(otwieranie okna)
	const bool running() const;

	//funkcje
	void pollEvents();
	void update();
	void render();
};

const bool Game::running() const
{
	return this->window->isOpen();
}

//konstruktory
void Game::iniVar()
{
	this->window = nullptr;
}

void Game::iniWin()
{
	this->videomode.height = 600;
	this->videomode.width = 800;

	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML animation", sf::Style::Default);
}

Game::Game()
{
	//taka kolejnoœæ bo na pocz¹tku chcemy pusty wskaŸnik
	this->iniVar();
	this->iniWin();
}

Game::~Game()
{
	delete this->window;
}

void Game::pollEvents()
{
	//event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;

		}
	}
}
void Game::update()
{
	this->pollEvents();
}

void Game::render()
{
	this->window->clear(sf::Color::Red);

	//rysowanie obiektow
	this->window->display();
}

int main()
{
	//init Game engine
	Game game;

	//Petla gry
	while (game.running())
	{
		//update
		game.update();

		//render
		game.render();

	}
	//koniec gry
	return 0;
}
//aktualnosci
//10.12.23 okno programu
//12.12.23 dodanie klas i przeniesienie pêtli gry poza funkcje main