#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <random>


namespace commands {
    std::string generate_artifact() {
        static const std::vector<std::string> prefixes = {
            "Ebon", "Crimson", "Shattered", "Cursed", "Forsaken", "Ancient", "Blighted", "Phantom"
        };

        static const std::vector<std::string> types = {
            "Amulet", "Blade", "Chalice", "Tome", "Crown", "Ring", "Scepter", "Mask"
        };

        static const std::vector<std::string> suffixes = {
            "of the Withered King", "of Eternal Night", "of the Lost Empire",
            "that Whispers Lies", "that Devours Light", "Bound in Shadow",
            "of Forgotten Echoes", "that Bleeds Time"
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<> dist_prefix(0, prefixes.size() - 1);
        std::uniform_int_distribution<> dist_type(0, types.size() - 1);
        std::uniform_int_distribution<> dist_suffix(0, suffixes.size() - 1);

        std::string artifact_name = prefixes[dist_prefix(gen)] + " " +
            types[dist_type(gen)] + " " +
            suffixes[dist_suffix(gen)];

        return artifact_name;
    }

    void register_artifact_command(dpp::cluster& bot) {
        bot.on_slashcommand([](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() != "artifact")
                return;

            std::string artifact = generate_artifact();
            std::string reply = "**Artifact Found:** " + artifact;
            event.reply(reply);
            });

        bot.on_ready([&bot](const dpp::ready_t& event) {
            if (dpp::run_once<struct register_artifact_command>()) {
                bot.global_command_create(
                    dpp::slashcommand("artifact", "Discover a mysterious artifact", bot.me.id)
                );
            }
            });
    }
}