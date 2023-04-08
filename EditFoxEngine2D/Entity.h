#ifndef __EDITFOXENGINE_ECS_ENTITY_H_
#define __EDITFOXENGINE_ECS_ENTITY_H_
#include <SFML/Graphics.hpp>
namespace EditFoxEngine {
	namespace ECS {
		class Entity
		{
		public:
			Entity();
			~Entity();
			void update(float deltaTime);
			void render(std::shared_ptr<sf::RenderTarget> target);
		};
	}
}
#endif
