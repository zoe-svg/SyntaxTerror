#include <fstream>
#include <ctime>
#include "../depends/json.hpp"
#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <random>

using json = nlohmann::json;

const std::string cycle_data_path = "data/cycle_data.json";

namespace commands {

    json load_cycle_data() {
        std::ifstream file(cycle_data_path);
        if (!file.is_open()) return json::object();
        json data;
        file >> data;
        return data;
    }

    void save_cycle_data(const json& data) {
        std::ofstream file(cycle_data_path);
        file << data.dump(4);
    }


    static std::string generate_cycle_lore() {
        static const std::vector<std::string> beginnings = {
            "Ashes drift through hollow spires.",
            "A kingdom of salt crumbles into memory.",
            "The stone-bound oath has cracked.",
            "Old gods shudder in sleep.",
            "A voice once buried begins to echo again.",
            "The harvest rots where no hands remain."
        };

        static const std::vector<std::string> middles = {
            "Worms chant beneath the roots.",
            "The blood of ancients runs cold.",
            "A new rot blossoms in familiar soil.",
            "Forgotten names are etched in bone.",
            "The watchers avert their gaze.",
            "The veil between lives thins once more."
        };

        static const std::vector<std::string> endings = {
            "Thus begins the turning.",
            "So falls another age.",
            "Cycle upon cycle, still nothing is learned.",
            "Let the next era inherit this ruin.",
            "Again, the wheel groans.",
            "The fruit of decay ripens anew."
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist_b(0, beginnings.size() - 1);
        std::uniform_int_distribution<> dist_m(0, middles.size() - 1);
        std::uniform_int_distribution<> dist_e(0, endings.size() - 1);

        return beginnings[dist_b(gen)] + " " +
            middles[dist_m(gen)] + " " +
            endings[dist_e(gen)];
    }

    void register_cycle_command(dpp::cluster& bot) {
        bot.on_slashcommand([](const dpp::slashcommand_t& event) {
            if (event.command.get_command_name() != "cycle")
                return;

            json data = load_cycle_data();
            std::string guild_id = std::to_string(event.command.guild_id);
            auto now = std::time(nullptr);

            // Random interval in seconds (4h to 3d)
            const int min_interval = 60 * 60 * 4;
            const int max_interval = 60 * 60 * 24 * 3;
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(min_interval, max_interval);

            if (!data.contains(guild_id)) {
                data[guild_id] = {
                    { "cycle", 1 },
                    { "last_change", now },
                    { "interval", dist(gen) },
                    { "lore", generate_cycle_lore() }
                };
                save_cycle_data(data);
            }

            int last = data[guild_id]["last_change"];
            int interval = data[guild_id]["interval"];
            int cycle = data[guild_id]["cycle"];

            if (now - last >= interval) {
                cycle++;
                data[guild_id]["cycle"] = cycle;
                data[guild_id]["last_change"] = now;
                data[guild_id]["interval"] = dist(gen);
                data[guild_id]["lore"] = generate_cycle_lore();
                save_cycle_data(data);
            }

            std::string reply = "**Cycle " + std::to_string(cycle) + "**\n" +
                data[guild_id]["lore"].get<std::string>();
            event.reply(reply);
            });

        bot.on_ready([&bot](const dpp::ready_t& event) {
            if (dpp::run_once<struct register_cycle_command>()) {
                bot.global_command_create(
                    dpp::slashcommand("cycle", "Turn the wheel of fate", bot.me.id)
                );
            }
            });
    }
}