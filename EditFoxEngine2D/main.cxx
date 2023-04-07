#include <argparse.hpp>
#include <memory>
#include "Game.h"
#include "xsstream.h"

int main(int argc, const char** argv) {
	xstringstream title;
	int width, height;
	std::unique_ptr<EditFoxEngine::Game> game;
	ArgumentParser parser;
	parser.addArgument("-w", "--width", 1, false);
	parser.addArgument("-h", "--height", 1, false);
	parser.addArgument("-n", "--name", '+', false);
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
	catch (std::bad_cast& e) {
		width = 1080;
		std::cout << e.what() << std::endl;
	}
	try {
		height = parser.retrieve<int>("height");
	}
	catch (std::bad_cast& e) {
		height = 720;
		std::cout << e.what() << std::endl;
	}
	try {
		game->initWindow(title.str(), width, height);
		game->run();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	return 0;
}