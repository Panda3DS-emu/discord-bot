#pragma once

#include "commands.hpp"

namespace poke {

    void Initialize();
    void Daily(const dpp::slashcommand_t& event);
    void Wish(const dpp::slashcommand_t& event, bool isMultiwishing);
    void MultiWish(const dpp::slashcommand_t& event);
    void Wishes(const dpp::slashcommand_t& event);
    void List(const dpp::slashcommand_t& event);
    void Favorite(const dpp::slashcommand_t& event);
    void Banner(const dpp::slashcommand_t& event);
    void Leaderboard(const dpp::slashcommand_t& event);
    void Roulette(const dpp::slashcommand_t& event);
    dpp::message ListPage(int page, uint64_t channel_id, uint64_t id);
    void ListNextPage(const dpp::button_click_t& event);
    void ListPrevPage(const dpp::button_click_t& event);

}
