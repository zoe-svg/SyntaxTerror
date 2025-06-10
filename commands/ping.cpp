#include <dpp/dpp.h>

namespace commands {
    void register_ping_command(dpp::cluster& bot) {
        // Slash command handler
        bot.on_slashcommand([](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() == "ping") {
                event.reply("Pong!");
            }
            });

        // Register the global slash command on bot ready
        bot.on_ready([&bot](const dpp::ready_t& event) {
            if (dpp::run_once<struct register_bot_commands>()) {
                bot.global_command_create(
                    dpp::slashcommand("ping", "Ping pong command", bot.me.id)
                );
            }
            });
    }
}