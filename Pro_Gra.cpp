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

class Gracz
{
private:
	sf::Sprite sprite;
	sf::Texture texture;

	void iniTexture();
	void iniSprite();

public:
	Gracz();
	virtual ~Gracz();

	//funkcje
	void update();
	void render(sf::RenderTarget& target);
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

	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Gra statek", sf::Style::Default);
}

Game::Game()
{
	//taka kolejno�� bo na pocz�tku chcemy pusty wska�nik
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

void Gracz::iniTexture()
{
	//Za�adowanie tekstury
}

void Gracz::iniSprite()
{
	this->sprite.setTexture(this->texture);
}

Gracz::Gracz()
{
	this->initTexture();
	this->initSprite();

}

Gracz::~Gracz()
{
	
}

void Gracz::update()
{
	
}

void Gracz::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
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
//12.12.23 dodanie klas i przeniesienie p�tli gry poza funkcje main
//26.12 dodanie klas gracza