#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>


namespace commands {
    static std::string generate_omen() {
        static const std::vector<std::string> openings = {
            "When the moon bleeds,", "As the crows gather,", "Upon the third eclipse,",
            "Should the altar crack,", "In the hour of still rot,", "When no bells toll,"
        };

        static const std::vector<std::string> subjects = {
            "the dead shall whisper", "kings will kneel in dust", "the black sun shall rise",
            "a nameless god shall wake", "the rot will bloom again", "your name will be forgotten"
        };

        static const std::vector<std::string> endings = {
            "and none shall be spared.", "until the last light fades.", "as the worms sing praises.",
            "and silence shall reign eternal.", "as old blood drowns the roots.", "while watchers close their eyes."
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist_opening(0, openings.size() - 1);
        std::uniform_int_distribution<> dist_subject(0, subjects.size() - 1);
        std::uniform_int_distribution<> dist_ending(0, endings.size() - 1);

        const std::string& part1 = openings[dist_opening(gen)];
        const std::string& part2 = subjects[dist_subject(gen)];
        const std::string& part3 = endings[dist_ending(gen)];

        return part1 + " " + part2 + ", " + part3;
    }

    void register_omen_command(dpp::cluster& bot) {
        // Slash command handler
        bot.on_slashcommand([](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() == "omen") {
                event.reply(generate_omen());
            }
            });

        // Register the global slash command on bot ready
        bot.on_ready([&bot](const dpp::ready_t& event) {
            if (dpp::run_once<struct register_omen_commands>()) {
                bot.global_command_create(
                    dpp::slashcommand("omen", "Receive a cryptic omen", bot.me.id)
                );
            }
            });
    }
}