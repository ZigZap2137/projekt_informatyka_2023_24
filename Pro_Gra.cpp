#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <fstream>
#include <map>
#include <time.h>
#include <windows.h>

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

	std::map<std::string, sf::Texture*> textures;
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
	void pollEvents();//zamykanie ESC
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
	//zmieniając atClMAX, w funkcji poniżej można zmienic opóźnienie kolejnego pocisku
	void initVariables();
	void initTexture();
	void initSprite();

public:
	Gracz();

	const sf::Vector2f& getPos() const;

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
	Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_x, float dir_y, float movement_speed);

	//krawędzie pocisku
	const sf::FloatRect getBounds() const;//zwraca globalne współrzędne elementu (co oznacza że uwzględnia skalowanie, rotacje,i inne przekształcenia)

	void update();
	void render(sf::RenderTarget* target);
};

class Enemy
{
private:
	sf::ConvexShape shape;

	void initShape();//kształt i kolor przeciwnika

public:

	Enemy(float pos_x, float pos_y);
	bool Coll(const Bullet& bullet);//kolizja pocisku i przeciwnika
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
	//współrzędnie przeciwnika
	this->enemy = new Enemy(600.f, 20.f);

}

Game::Game()
{
	//początku pusty wskaźnik
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

	delete this->enemy;

}

void Enemy::initShape()
{
	this->shape.setPointCount(5);
	this->shape.setPoint(0, sf::Vector2f(0.f, 0.f));
	this->shape.setPoint(1, sf::Vector2f(150.f, 10.f));
	this->shape.setPoint(2, sf::Vector2f(120.f, 90.f));
	this->shape.setPoint(3, sf::Vector2f(30.f, 100.f));
	this->shape.setPoint(4, sf::Vector2f(0.f, 50.f));

	this->shape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	this->shape.setOutlineThickness(10.f);
	this->shape.setOutlineColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));

}

bool Enemy::Coll(const Bullet& bullet)
{
	return this->shape.getGlobalBounds().intersects(bullet.getBounds());
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
		//pierwsza wartość odchylenie lewo/prawo, druga kierunek góra/dół, trzecia prędkość pocisku; ustawienia strzelania
		this->bullets.push_back(new Bullet(this->textures["BULLET"], this->gracz->getPos().x, this->gracz->getPos().y, 0.f, -1.f, 1.0f));
	}

}

void Game::updateBullets()
{
	int licznik = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();

		//wyjście pocisku poza górę okna
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//usuwanie pocisku
			delete this->bullets.at(licznik);
			this->bullets.erase(this->bullets.begin() + licznik);
			--licznik;
		}
		else
		{
			// Sprawdź kolizję z przeciwnikiem
			if (this->enemy->Coll(*bullet))
			{
				// Usuń pocisk i przeciwnika
				delete this->bullets.at(licznik);
				this->bullets.erase(this->bullets.begin() + licznik);
				delete this->enemy;
				this->enemy = nullptr;

				this->window->close();

				break; // Przerwij pętlę, ponieważ przeciwnik został zniszczony
			}
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

	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	if (this->enemy != nullptr)
	{
		this->enemy->render(this->window);
	}
	this->window->display();
}

void Gracz::initVariables()
{
	this->movementSpeed = 1.f;
	this->attackCooldownMAX = 20.f;
	this->attackCooldown = this->attackCooldownMAX;
}

void Gracz::initTexture()
{
	//(komunikat na wypadek nie załądownie poprawnie tekstury)
	if (!this->texture.loadFromFile("textures/spaceship.png"))
	{
		cout << "ERROR::PLAYER::INITTEXTURE::Nie było możliwości załadować pliku tekstury. " << "\n";
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

Bullet::Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_x, float dir_y, float movement_speed)
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

const sf::Vector2f& Gracz::getPos() const
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

typedef struct {
	//gracz moze wybrać według standardu, maksymalnie nazwe o długości 16 znaków
	char Nazwa[15];
	int Id;
} student;

int main()
{
	student* dane;
	dane = (student*)malloc(50 * sizeof(student));//tabl 50 pracownikow
	printf("Nazwa: ");
	scanf("%s", &dane[0].Nazwa);
	printf("Id: ");
	scanf("%d", &dane[0].Id);
	printf("Nazwa: %s\n", dane[0].Nazwa);
	printf("Indeks Studenta: %d\n", dane[0].Id);
	//chce by w pliku Dane_gracza, dane były cały czas dopisywane, przy każdym otwarciu gry
	FILE* file = fopen("Dane_gracza.txt", "a+");
	if(file != NULL)
	{
		fprintf(file, "Nazwa: %s\n", dane[0].Nazwa);
		fprintf(file, "Indeks studenta: %d\n", dane[0].Id);
		fclose(file);
	}
	free(dane);

	srand(time(NULL));
	Game game;

	//Pętla gry
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
//12.12.23 dodanie klas i przeniesienie pętli gry(pętla pollevents) poza funkcje main
//26.12 dodanie klasy gracza
//28.12 dodanie sprite'a gracza, i dodanie możliwości graczowi ruchu
//28.12 dodanie pocisków do gry
//29.12 pociski się poruszają, jest opóźnienie po każdym strzale(jest kolizja granicy okna i pocisku), dodanie przeciwnika
//02.01 small fixes, kolor przeciwnika jest generowany losowo przy otwarciu gry
//06.01 dodanie funkcji zwracającej kolizjie pocisku i przeciwnika(Coll), dodanie do UpdateBullets, fragmentu odpowiedzialnego za usuwanie przeciwnika po wykryciu kolizji i zamknięcie po tym okna z grą
//13.01 Dodanie: zapisu do pliku nazwy gracza i indeksu studenta z tablicy struktur (dane podaje gracz z klawiatury w oknie konsoli), wykorzystując przy tym, dynamiczną alokacje pamięci mallock