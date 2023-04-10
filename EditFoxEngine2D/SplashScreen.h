#pragma once
#include "State.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
namespace EditFoxEngine {
    class Game;
    namespace States {
        class SplashScreen :
            public State
        {
            const Game& game;
        public:
            explicit SplashScreen(FiniteStateMachine& fsm, xstring name);
            virtual void enter() override;
            virtual void update() override;
            virtual void exit() override;
        private:
            sf::RenderWindow* renderingSpace;
            sf::Sprite* logoSprite;
            sf::Text* efeWatermark;
        };
    }
}
