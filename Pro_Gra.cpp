#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <time.h>

using namespace std;

class Enemy;

class Bullet;

class Gracz;

class Game
{
private:
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	std::map<std:: string, sf::Texture*> textures;
	std::vector<Bullet*>bullets;

	//Gracz
	Gracz* gracz;

	//przeciwnik
	Enemy* enemy;

	void iniWin();//okna
	void initTextures();
	void iniGracz();
public:

	Game();
	virtual ~Game();
	//otwieranie okna
	const bool running() const;

	//funkcje
	void pollEvents();
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

	float attackCooldown;
	float attackCooldownMAX;

	void initVariables();
	void initTexture();
	void initSprite();

public:
	Gracz();

	const sf::Vector2f& getPods() const;

	//funkcje
	void move(const float dirX, const float dirY);
	const bool canAttack();

	void updateAttack();
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
	Bullet(sf::Texture * texture, float pos_x, float pos_y, float dir_x, float dir_y, float movement_speed);

	//krawêdzie pocisku
	const sf::FloatRect getBounds() const;

	void update();
	void render(sf::RenderTarget* target);
};

class Enemy
{
private:
	sf::ConvexShape shape;

	void initShape();

public:
	//krawêdzie przeciwnika


	Enemy(float pos_x, float pos_y);
	void render(sf::RenderTarget* target);
};

const bool Game::running() const
{
	return this->window->isOpen();
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
	texture->loadFromFile("textures/Bu.png");
	this->textures["BULLET"] = texture;

}

void Game::iniGracz()
{
	this->gracz = new Gracz();
	//wspó³rzêdnie przeciwnika
	this->enemy = new Enemy(600.f, 20.f);

}

Game::Game()
{
	//taka kolejnoœæ bo na pocz¹tku chcemy pusty wskaŸnik
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
	//usuwanie pocisków
	for (auto* i : this->bullets)
	{
		delete i;
	}

}

void Enemy::initShape()
{
	this->shape.setPointCount(5);
	this->shape.setPoint(0, sf::Vector2f(0.f, 0.f));
	this->shape.setPoint(1, sf::Vector2f(150.f, 10.f));
	this->shape.setPoint(2, sf::Vector2f(120.f, 90.f));
	this->shape.setPoint(3, sf::Vector2f(30.f, 100.f));
	this->shape.setPoint(4, sf::Vector2f(0.f, 50.f));

	this->shape.setFillColor(sf::Color(150, 50, 250));
	this->shape.setOutlineThickness(10.f);
	this->shape.setOutlineColor(sf::Color(250, 150, 100));

}


Enemy::Enemy(float pos_x, float pos_y)
{
	this->initShape();

	this->shape.setPosition(pos_x, pos_y);

}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
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

void Game::updateInput()
{
	//ruch gracza
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->gracz->move(-1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->gracz->move(1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->gracz->move(0, -1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->gracz->move(0, 1);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->gracz->canAttack())
	{
		//pierwsza wartoœæ odchylenie lewo/prawo, druga kierunek góra/dó³, trzecia prêdkoœæ pocisku; ustawienia strzelania
		this->bullets.push_back(new Bullet(this->textures["BULLET"],this->gracz->getPods().x, this->gracz->getPods().y, 0.f, -1.f, 1.0f));
	}

}

void Game::updateBullets()
{
	int licznik = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();
		
		//wyjœcie pocisku poza górê okna
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//usuwanie pocisku
			delete this->bullets.at(licznik);
			this->bullets.erase(this->bullets.begin() + licznik);
			--licznik;
		}
		++licznik;
	}
}

void Game::update()
{
	this->pollEvents();
	
	this->updateInput();

	this->gracz->update();

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

	this->enemy->render(this->window);

	this->window->display();
}

void Gracz::initVariables()
{
	this->movementSpeed = 1.f;
	this->attackCooldownMAX = 10.f;
	this->attackCooldown = this->attackCooldownMAX;
}

void Gracz::initTexture()
{
	//(komunikat na wypadek nie za³¹downie poprawnie tekstury)
	if (!this->texture.loadFromFile("textures/spaceship.png"))
	{
		cout << "ERROR::PLAYER::INITTEXTURE::Nie by³o mo¿liwoœci za³adowaæ pliku tekstury. " << "\n";
	}


}

void Gracz::initSprite()
{
	this->sprite.setTexture(this->texture);

	//skalowanie sprite'a gracza
	this->sprite.scale(0.4f, 0.4f);

}

Gracz::Gracz()
{
	this->initVariables();

	this->initTexture();
	this->initSprite();

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

const bool Gracz::canAttack()
{
	if (this->attackCooldown >= this->attackCooldownMAX)
	{
		this->attackCooldown = 0.f;
		return true;
	}
	return false;
	
}

void Gracz::updateAttack()
{
	if (this->attackCooldown < this->attackCooldownMAX)
		this->attackCooldown += 0.1f;
}

void Gracz::update()
{
	this->updateAttack();
}

void Gracz::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

int main()
{
	srand(time(NULL));
	//Wywo³anie silnika gry
	Game game;

	//Pêtla gry
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
//26.12 dodanie klasy gracza
//28.12 dodanie sprite'a gracza, i dodanie mo¿liwoœci graczowi ruchu
//28.12 dodanie pocisków do gry
//29.12 pociski siê poruszaj¹, jest opóŸnienie po ka¿dym strzale, dodanie przeciwnika