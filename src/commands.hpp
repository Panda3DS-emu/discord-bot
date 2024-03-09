#pragma once

#include <dpp/dpp.h>

namespace commands {
    using CommandFunction = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;

    void Settings(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Download(dpp::cluster& bot, const dpp::slashcommand_t& event);
}