#include "stdafx.h"

#include "Game.h"

EditFoxEngine::Game::Game()
{
	this->window = new sf::Window();
}

EditFoxEngine::Game::~Game()
{
	delete this->window;
}

void EditFoxEngine::Game::initWindow(xstring name, int width, int height)
{
	this->window->create(sf::VideoMode(width,height),std::string(name.c_str()));
}

sf::WindowHandle EditFoxEngine::Game::handle() const&&
{
	return this->window->getSystemHandle();
}

void EditFoxEngine::Game::updateSFMLEvents()
{
	while (this->window->pollEvent(this->ev)) {
		if (this->ev.type == sf::Event::Closed)
			this->window->close();
	}
}

void EditFoxEngine::Game::update()
{
	window->display();
}

void EditFoxEngine::Game::run()
{
	while (this->window->isOpen()) {
		this->updateSFMLEvents();
		this->update();
	}
}
