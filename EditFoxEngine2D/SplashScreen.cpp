#include "stdafx.h"
#include "SplashScreen.h"

extern std::unique_ptr<EditFoxEngine::Game> game;

EditFoxEngine::States::SplashScreen::SplashScreen(FiniteStateMachine& fsm, xstring name)
	: State(fsm,name)
{
}

void EditFoxEngine::States::SplashScreen::enter()
{
	this->renderingSpace = game->handle();
	auto screenSize = this->renderingSpace->getSize();
	this->efeWatermark.setFont(Game::getFont("avrile-sans"));
	this->efeWatermark.setString("Made with Edit Fox Engine");
	this->efeWatermark.setCharacterSize(16);
	this->efeWatermark.setStyle(sf::Text::Regular);
	this->efeWatermark.setFillColor(sf::Color::White);
	this->renderingSpace->draw(this->efeWatermark);
	auto watermarkSize = this->efeWatermark.getLocalBounds();
	this->efeWatermark.setOrigin(watermarkSize.width/2,watermarkSize.height/2);
	this->efeWatermark.setPosition(screenSize.x / 2, screenSize.y - (watermarkSize.height / 2));
	auto defColour = sf::Color::White;
	defColour.a = 0;
	this->logoSprite.setColor(defColour);
#ifdef _DEBUG
	logo.loadFromFile("./content/logo.png");
	this->logoSprite.setTexture(logo,true);
#else
	// TODO: LZMA2 archive loading implementation for Release
#endif
	sf::IntRect logoRect = this->logoSprite.getTextureRect();
	this->logoSprite.setOrigin(logoRect.width / 2, logoRect.height / 2);
	this->logoSprite.setPosition(screenSize.x / 2, screenSize.y / 2);
}

void EditFoxEngine::States::SplashScreen::update()
{
	auto dt = game->deltaTime();
	this->renderingSpace->draw(this->efeWatermark);
	auto colour = this->logoSprite.getColor();
	colour.a = floor(sin(this->splashAnimation)*0xff);
	this->logoSprite.setColor(colour);
	this->renderingSpace->draw(this->logoSprite);
	this->splashAnimation += dt;
}

void EditFoxEngine::States::SplashScreen::exit()
{
	delete this->renderingSpace;
}
