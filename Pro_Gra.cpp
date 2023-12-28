#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>

using namespace std;

class Bullet;

class Gracz;

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	//Resources
	std::map<std:: string, sf::Texture*> textures;
	std::vector<Bullet*>bullets;

	//Gracz
	Gracz* gracz;

	void iniVar();//zmiennych
	void iniWin();//okna
	void initTextures();
	void iniGracz();
public:
	//konstruktory
	Game();
	virtual ~Game();

	//dostepy(otwieranie okna)
	const bool running() const;

	//funkcje
	void pollEvents();
	void run();

	void updatePollEvents();
	void updateInput();
	void updateBullets();
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

	//accessor
	const sf::Vector2f& getPods() const;

	//funkcje
	void move(const float dirX, const float dirY);

	void update();
	void render(sf::RenderTarget& target);
};

class Bullet
{
private:
	sf::Sprite shape;
	sf::Texture* texture;

	sf::Vector2f direction;
	float movementSpeed;
public:
	Bullet();
	Bullet(sf::Texture * texture, float pos_x, float pos_y, float dir_x, float dir_y, float movement_speed);
	virtual ~Bullet();

	//Accessor
	const sf::FloatRect getBounds() const;

	void update();
	void render(sf::RenderTarget* target);
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

void Game::initTextures()
{
	sf::Texture* texture = new sf::Texture();
	texture->loadFromFile("textures/Bullet.png");
	this->textures["BULLET"] = texture;

}

void Game::iniGracz()
{
	this->gracz = new Gracz();
}

Game::Game()
{
	//taka kolejno�� bo na pocz�tku chcemy pusty wska�nik
	this->iniVar();
	this->iniWin();
	this->initTextures();
	this->iniGracz();
}

Game::~Game()
{
	delete this->window;
	delete this->gracz;

	//usuwanie tekstur
	for (auto& i : this->textures)
	{
		delete i.second;
	}
	//usuwanie pocisk�w
	for (auto* i : this->bullets)
	{
		delete i;
	}

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

void Game::run()
{
	;
}

void Game::updatePollEvents()
{
	;
}

void Game::updateInput()
{
	//ruch gracza
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->gracz->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->gracz->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->gracz->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->gracz->move(0.f, 1.f);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->bullets.push_back(new Bullet(this->textures["Bullet"],this->gracz->getPods().x, this->gracz->getPods().y, 0.f, 0.f, 0.f, 0.f, 0.f));
	}

}

void Game::updateBullets()
{
	for (auto* bullet : this->bullets)
	{
		bullet->update();
	}
}

void Game::update()
{
	this->pollEvents();
	this->updateInput();
	this->updateBullets();
}

void Game::render()
{
	this->window->clear(sf::Color::Black);

	//rysowanie obiektow
	this->gracz->render(*this->window);

	for (auto *bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	this->window->display();
}

void Gracz::initTexture()
{
	//Za�adowanie tekstury
	if (!this->texture.loadFromFile("textures/spaceship.png"))
	{
		cout << "ERROR::PLAYER::INITTEXTURE::Nie by�o mo�liwo�ci za�adowa� pliku tekstury. " << "\n";
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

Bullet::Bullet()
{

}

Bullet::Bullet(sf::Texture * texture, float pos_x, float pos_y, float dir_x, float dir_y, float movement_speed)
{
	this->texture = texture;
	this->shape.setTexture(*texture);

	this->shape.setPosition(pos_x, pos_y);
	this->direction.x = dir_x;
	this->direction.y = dir_y;
	this->movementSpeed = movement_speed;


}

Bullet::~Bullet()
{

}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

void Bullet::update()
{
	this->shape.move(this->movementSpeed * this->direction);
}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}

const sf::Vector2f& Gracz::getPods() const
{
	return this->sprite.getPosition();
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
//12.12.23 dodanie klas i przeniesienie p�tli gry poza funkcje main
//26.12 dodanie klas gracza
//28.12 dodanie sprite'a gracza, i ddanie mo�liwo�ci graczowi ruchu
//28.12 dodanie pocisk�w do gry
