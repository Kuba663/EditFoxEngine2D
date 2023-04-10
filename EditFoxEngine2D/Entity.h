#ifndef __EDITFOXENGINE_ECS_ENTITY_H_
#define __EDITFOXENGINE_ECS_ENTITY_H_
#include <SFML/Graphics.hpp>
#include "efeid.h"
#include "scapegoat.h"
namespace EditFoxEngine {
	namespace ECS {
		class Entity
		{
			friend class scapegoat<EditFoxEngine::efeid, EditFoxEngine::ECS::Entity, std::hash<EditFoxEngine::efeid>, std::less<size_t>, stl_allocator<EditFoxEngine::ECS::Entity>>;
			efeid id;
		public:
			Entity();
			~Entity();
			void update(float deltaTime);
			void render(std::shared_ptr<sf::RenderTarget> target);
		};
	}
}
#endif
