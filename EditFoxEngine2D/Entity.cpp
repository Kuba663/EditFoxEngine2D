#include "stdafx.h"
#include "Entity.h"

extern Allocator* entity_allocator;

EditFoxEngine::ECS::Entity::Entity()
{
}

EditFoxEngine::ECS::Entity::~Entity()
{
}

void EditFoxEngine::ECS::Entity::update(float deltaTime)
{
}

void EditFoxEngine::ECS::Entity::render(std::shared_ptr<sf::RenderTarget> target)
{
}

void* EditFoxEngine::ECS::Entity::operator new(size_t size)
{
	return entity_allocator->Allocate(size);
}

void EditFoxEngine::ECS::Entity::operator delete(void* pObject)
{
	entity_allocator->Deallocate(pObject);
}
