#pragma once

#include "commands.hpp"
#include <functional>

namespace admins {
    // Runs a command callback after first checking the user has admin priviledges
    void AdminCommand(const commands::CommandFunction& command, dpp::cluster& bot, const dpp::slashcommand_t& event);

    void Initialize();
    void AddAdmin(const dpp::slashcommand_t& event, const dpp::guild_member& member);
    void RemoveAdmin(const dpp::slashcommand_t& event, const dpp::guild_member& member);
}