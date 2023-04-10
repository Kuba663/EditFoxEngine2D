#include "stdafx.h"
#include "SplashScreen.h"

void EditFoxEngine::States::SplashScreen::enter()
{
	this->renderingSpace = new sf::RenderWindow(game.handle());
	auto screenSize = this->renderingSpace->getSize();
	this->logoSprite = new sf::Sprite();
	this->efeWatermark = new sf::Text("Made with Edit Fox Engine", game.getFont("avrile-sans"),10);
	this->efeWatermark->setFillColor(sf::Color::White);
	auto watermarkSize = this->efeWatermark->getLocalBounds();
	this->efeWatermark->setOrigin(watermarkSize.width/2,watermarkSize.height/2);
	this->efeWatermark->setPosition(screenSize.x / 2, screenSize.y - (watermarkSize.height / 2));
#ifdef _DEBUG
	sf::FileInputStream stream;
	sf::Texture logo;
	stream.open("/content/logo.png");
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
	this->renderingSpace->clear();
	this->renderingSpace->draw(*this->efeWatermark);
	this->renderingSpace->draw(*this->logoSprite);
	this->renderingSpace->display();
}

void EditFoxEngine::States::SplashScreen::exit()
{
	delete this->renderingSpace;
	delete this->logoSprite;
	delete this->efeWatermark;
}
