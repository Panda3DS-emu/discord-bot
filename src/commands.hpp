#pragma once

#include <dpp/dpp.h>

namespace commands {
    using CommandFunction = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;

    void Settings(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveAdmin(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AddMacroFile(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void RemoveMacro(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Download(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void ClearCache(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void StartPuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void SolvePuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void CurrentPuzzle(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Panda(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Image(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void AskPanda(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void SetPrompt(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void ResetPrompt(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void HiddenSay(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void HiddenFile(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void History(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void ClearContext(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Wishes(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Wish(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Daily(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void List(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Favorite(dpp::cluster& bot, const dpp::slashcommand_t& event);
    void Banner(dpp::cluster& bot, const dpp::slashcommand_t& event);
}