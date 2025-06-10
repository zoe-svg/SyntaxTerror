#pragma once
#include <dpp/dpp.h>

namespace commands {
	void register_ping_command(dpp::cluster& bot);
	void register_omen_command(dpp::cluster& bot);
	void register_cycle_command(dpp::cluster& bot);
	void register_corrupt_command(dpp::cluster& bot);
	void register_artifact_command(dpp::cluster& bot);
}