#pragma once
#include "State.h"
#include "Game.h"
#include <SFML/Graphics.hpp>
namespace EditFoxEngine {
    namespace States {
        class SplashScreen :
            public State
        {
            friend class EditFoxEngine::Game;
            static const Game& game;
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
