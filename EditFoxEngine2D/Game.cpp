#include "stdafx.h"

#include "Game.h"

//IMPLEMENT_ALLOCATOR(EditFoxEngine::Game, 1, NULL)

scapegoat<xstring, sf::Font, std::hash<xstring>, std::less<size_t>, std::allocator<sf::Font>> EditFoxEngine::Game::fontRegistry = scapegoat<xstring, sf::Font,std::hash<xstring>,std::less<size_t>,std::allocator<sf::Font>>();

EditFoxEngine::Game::Game()
{
	this->window = new sf::RenderWindow();
	this->gameStateMachine = new FiniteStateMachine();
#ifdef _DEBUG
	sf::FileInputStream stream;
	stream.open("./content/fonts/AvrileSans.ttf");
	sf::Font f;
	f.loadFromStream(stream);
	Game::fontRegistry.insert("avrile-sans", f);
#else
	// TODO: LZMA2 archive loading implementation for Release
#endif
	this->gameStateMachine->addState<EditFoxEngine::States::SplashScreen>("splash");
}

EditFoxEngine::Game::~Game()
{
	delete this->gameStateMachine;
	delete this->window;
}

void EditFoxEngine::Game::initWindow(xstring name, int width, int height)
{
	this->window->create(sf::VideoMode(width,height),std::string(name.c_str()));
	
	this->gameStateMachine->setCurrentState("splash");
}

sf::RenderTarget* EditFoxEngine::Game::handle() const
{
	return this->window;
}

sf::Font& EditFoxEngine::Game::getFont(xstring fontName)
{
	return Game::fontRegistry[fontName];
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
	window->clear();
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
