#include <dpp/dpp.h>
#include <string>
#include <random>


namespace commands {
    std::string corrupt_text(const std::string& input) {
        std::string corrupted;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> glitch_dist(0, 99);
        std::uniform_int_distribution<> char_glitch(0, 4);

        for (char c : input) {
            if (glitch_dist(gen) < 20) {
                // Random corruption: replace with glitch character
                static const std::vector<std::string> glitches = { "░", "▓", "█", "☠", "⛧", "~", "⍦" };
                corrupted += glitches[glitch_dist(gen) % glitches.size()];
            }
            else if (glitch_dist(gen) < 30) {
                // Duplicate character
                corrupted += c;
                corrupted += c;
            }
            else if (glitch_dist(gen) < 40) {
                // Random case mutation
                corrupted += (std::islower(c) ? std::toupper(c) : std::tolower(c));
            }
            else {
                // Normal character
                corrupted += c;
            }
        }

        return corrupted;
    }

    void register_corrupt_command(dpp::cluster& bot) {
        bot.on_slashcommand([](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() != "corrupt")
                return;

            std::string input = std::get<std::string>(event.get_parameter("text"));
            if (input.length() > 200) {
                event.reply("The corruption cannot grasp such vast thoughts. Try something shorter.");
                return;
            }

            std::string result = corrupt_text(input);
            event.reply("`" + result + "`");
            });

        bot.on_ready([&bot](const dpp::ready_t& event) {
            if (dpp::run_once<struct register_corrupt_command>()) {
                bot.global_command_create(
                    dpp::slashcommand("corrupt", "Submit a phrase to decay and distortion", bot.me.id)
                    .add_option(dpp::command_option(dpp::co_string, "text", "The message to corrupt", true))
                );
            }
            });
    }
}