#include "game/Game.h"
#include <iostream>
#include <exception>

int main(int argc, char* argv[]) {
    try {
        Game::Game game;
        
        if (!game.init()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }
        
        game.run();
        game.shutdown();
        
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred!" << std::endl;
        return 1;
    }
}
