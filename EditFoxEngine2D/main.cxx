#include <argparse.hpp>
#include <memory>
#include "Game.h"
#include <xsstream.h>
#include "scapegoat.h"
#include "Entity.h"
#define COMMA ,

IMPLEMENT_ALLOCATOR(scapegoat<EditFoxEngine::efeid COMMA EditFoxEngine::ECS::Entity COMMA std::hash<EditFoxEngine::efeid> COMMA std::less<size_t> COMMA stl_allocator<EditFoxEngine::ECS::Entity>>, 0, NULL)

char* entity_heap;
Allocator* entity_allocator;

int main(int argc, const char** argv) {
	xstringstream title;
	int width, height, entityCount;
	std::unique_ptr<EditFoxEngine::Game> game;
	ArgumentParser parser;
	parser.addArgument("-w", "--width", 1, false);
	parser.addArgument("-h", "--height", 1, false);
	parser.addArgument("-n", "--name", '+', false);
	parser.addArgument("-e", "--maxentitycount", 1, false);
	parser.parse(argc, argv);
	game = std::make_unique<EditFoxEngine::Game>();
	try {
		auto strings = parser.retrieve<std::vector<std::string>>("name");
		for (auto str : strings)
			title << str << " ";
	}
	catch (std::exception& e) {
		title << argv[0];
		std::cout << e.what() << std::endl;
	}
	try {
		width = parser.retrieve<int>("width");
	}
	catch (std::exception& e) {
		width = 1080;
		std::cout << e.what() << std::endl;
	}
	try {
		height = parser.retrieve<int>("height");
	}
	catch (std::exception& e) {
		height = 720;
		std::cout << e.what() << std::endl;
	}
	try {
		entityCount = parser.retrieve<int>("maxentitycount");
	}
	catch (std::exception& e) {
		entityCount = 1024;
		std::cout << e.what() << std::endl;
	}
	try {
		if (entityCount != 0) entity_heap = (char*)xmalloc(sizeof(EditFoxEngine::ECS::Entity) * (entityCount + 1));
		else entity_heap = NULL;
		entity_allocator = new Allocator(sizeof(EditFoxEngine::ECS::Entity), entityCount, entity_heap, "Entity");
		game->initWindow(title.str(), width, height);
		game->run();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		delete entity_allocator;
		if (entity_heap != NULL) xfree(entity_heap);
		return -1;
	}
	delete game.get();
	delete entity_allocator;
	if (entity_heap != NULL) xfree(entity_heap);
	return 0;
}