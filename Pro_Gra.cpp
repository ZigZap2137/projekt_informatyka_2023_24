#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>

using namespace std;

class Gracz;

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	//Gracz
	Gracz* gracz;

	void iniVar();//zmiennych
	void iniWin();//okna
	void iniGracz();
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

	float movementSpeed;

	void initTexture();
	void initSprite();

public:
	Gracz();
	virtual ~Gracz();

	//funkcje
	void move(const float dirX, const float dirY);

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
	this->videomode.height = 960;
	this->videomode.width = 1280;

	this->window = new sf::RenderWindow(sf::VideoMode(1280, 960), "Gra statek", sf::Style::Default);
}

void Game::iniGracz()
{
	this->gracz = new Gracz();
}

Game::Game()
{
	//taka kolejnoœæ bo na pocz¹tku chcemy pusty wskaŸnik
	this->iniVar();
	this->iniWin();
	this->iniGracz();
}

Game::~Game()
{
	delete this->window;
	delete this->gracz;

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
	//ruch gracza
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->gracz->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->gracz->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->gracz->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->gracz->move(0.f, 1.f);
}

void Game::update()
{
	this->pollEvents();
}

void Game::render()
{
	this->window->clear(sf::Color::Green);

	//rysowanie obiektow
	this->gracz->render(*this->window);
	this->window->display();
}

void Gracz::initTexture()
{
	//Za³adowanie tekstury
	if (!this->texture.loadFromFile("textures/spaceship.png"))
	{
		cout << "ERROR::PLAYER::INITTEXTURE::Nie by³o mo¿liwoœci za³adowaæ pliku tekstury. " << "\n";
	}


}

void Gracz::initSprite()
{
	this->sprite.setTexture(this->texture);

	//resize sprite
	this->sprite.scale(0.4f, 0.4f);

}

Gracz::Gracz()
{
	this->movementSpeed = 1.f;

	this->initTexture();
	this->initSprite();

}

Gracz::~Gracz()
{

}

void Gracz::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
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
//12.12.23 dodanie klas i przeniesienie pêtli gry poza funkcje main
//26.12 dodanie klas gracza
//28.12 dodanie sprite'a gracza, i ddanie mo¿liwoœci graczowi ruchu