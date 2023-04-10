#include "stdafx.h"
#ifndef __EFE_GAME_H_
#define __EFE_GAME_H_
#include <SFML/Graphics.hpp>
#include "FiniteStateMachine.h"
#include "scapegoat.h"
#include "SplashScreen.h"
namespace EditFoxEngine {
	class Game
	{
	private:
		sf::RenderWindow* window;
		sf::Event ev;
		FiniteStateMachine* gameStateMachine;
		static scapegoat<xstring, sf::Font> fontRegistry;
	public:
		Game();
		~Game();
		void initWindow(xstring name,int width, int height);
		sf::RenderTarget* handle() const;
		static sf::Font& getFont(xstring fontName);
		void updateSFMLEvents();
		void update();
		void run();
		DECLARE_ALLOCATOR
	};
}
#endif
