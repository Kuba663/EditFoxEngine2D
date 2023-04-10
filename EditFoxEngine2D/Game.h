#include "stdafx.h"
#ifndef __EFE_GAME_H_
#define __EFE_GAME_H_
#include <SFML/Graphics.hpp>
#include "FiniteStateMachine.h"
#include "scapegoat.h"
namespace EditFoxEngine {
	class Game
	{
	private:
		sf::Window* window;
		sf::Event ev;
		FiniteStateMachine* gameStateMachine;
		scapegoat<xstring, sf::Font> fontRegistry;
	public:
		Game();
		~Game();
		void initWindow(xstring name,int width, int height);
		sf::WindowHandle handle() const&;
		sf::Font& getFont(xstring fontName) const&;
		void updateSFMLEvents();
		void update();
		void run();
		DECLARE_ALLOCATOR
	};
}
#endif
