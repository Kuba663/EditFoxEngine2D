#include "stdafx.h"

#include "Game.h"

IMPLEMENT_ALLOCATOR(EditFoxEngine::Game,1,NULL)

EditFoxEngine::Game::Game()
{
	this->window = new sf::Window();
	this->gameStateMachine = new FiniteStateMachine();
}

EditFoxEngine::Game::~Game()
{
	delete this->gameStateMachine;
	delete this->window;
}

void EditFoxEngine::Game::initWindow(xstring name, int width, int height)
{
	this->window->create(sf::VideoMode(width,height),std::string(name.c_str()));
}

sf::WindowHandle EditFoxEngine::Game::handle() const&
{
	return this->window->getSystemHandle();
}

sf::Font& EditFoxEngine::Game::getFont(xstring fontName) const&
{
	return this->fontRegistry[fontName];
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
	this->gameStateMachine->update();
	window->display();
}

void EditFoxEngine::Game::run()
{
	while (this->window->isOpen()) {
		this->updateSFMLEvents();
		this->update();
	}
}
