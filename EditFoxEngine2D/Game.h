#include "stdafx.h"
#ifndef __EFE_GAME_H_
#define __EFE_GAME_H_
#include <SFML/Graphics.hpp>
#include "FiniteStateMachine.h"
namespace EditFoxEngine {
	class Game
	{
	private:
		sf::Window* window;
		sf::Event ev;
		FiniteStateMachine* gameStateMachine;
	public:
		Game();
		~Game();
		void initWindow(xstring name,int width, int height);
		sf::WindowHandle handle() const&&;
		void updateSFMLEvents();
		void update();
		void run();
	};
}
#endif
