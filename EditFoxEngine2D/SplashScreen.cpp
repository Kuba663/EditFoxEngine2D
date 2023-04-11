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
	this->logoSprite = new sf::Sprite();
	auto font = Game::getFont("avrile-sans");
	this->efeWatermark = new sf::Text("Made with Edit Fox Engine", font,10);
	this->efeWatermark->setFillColor(sf::Color::White);
	auto watermarkSize = this->efeWatermark->getLocalBounds();
	this->efeWatermark->setOrigin(watermarkSize.width/2,watermarkSize.height/2);
	this->efeWatermark->setPosition(screenSize.x / 2, screenSize.y - (watermarkSize.height / 2));
#ifdef _DEBUG
	sf::FileInputStream stream;
	sf::Texture logo;
	stream.open("./content/logo.png");
	logo.loadFromStream(stream);
	this->logoSprite->setTexture(logo,true);
#else
	// TODO: LZMA2 archive loading implementation for Release
#endif
	sf::IntRect logoRect = this->logoSprite->getTextureRect();
	this->logoSprite->setOrigin(logoRect.width / 2, logoRect.height / 2);
	this->logoSprite->setPosition(screenSize.x / 2, screenSize.y / 2);
}

void EditFoxEngine::States::SplashScreen::update()
{
	this->renderingSpace->draw(*this->efeWatermark);
	this->renderingSpace->draw(*this->logoSprite);
}

void EditFoxEngine::States::SplashScreen::exit()
{
	delete this->renderingSpace;
	delete this->logoSprite;
	delete this->efeWatermark;
}
