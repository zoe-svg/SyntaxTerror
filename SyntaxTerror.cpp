//Externals
#include <dpp/dpp.h>
#include "depends/dotenv.h"

// Essentials
#include <string>
#include <iostream>
#include <memory>
#include <stdexcept>

//Commands
#include "commands/commands.h"

int main() {
    std::unique_ptr<std::string> api_token;


    try {
        
        dotenv::init();

        const std::string token = dotenv::getenv("API_TOKEN");
        if (token.empty()) {
            throw std::runtime_error("API_TOKEN returned empty.\n");
            std::cout << "bot execution stopped or errored press enter to exit.\n";
            std::cin.get();
            return 1;

        }
        std::cout << "API_TOKEN retrieved successfully\n";
        api_token = std::make_unique<std::string>(token);
    }
    catch (const std::exception& e) {
        std::cerr << "Error Loading token: " << e.what() << std::endl;
        std::cout << "Bot execution stopped or errored press enter to exit.\n";
        std::cin.get();
        return 1;
    }

    std::cout << "initializing discord login...\n";
    dpp::cluster bot(*api_token);
    bot.on_log(dpp::utility::cout_logger());

    commands::register_ping_command(bot);
    commands::register_omen_command(bot);
    commands::register_cycle_command(bot);
    commands::register_corrupt_command(bot);
    commands::register_artifact_command(bot);


    bot.start(dpp::st_wait);
}
