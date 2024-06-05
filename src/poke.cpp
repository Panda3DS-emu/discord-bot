#include "poke.hpp"
#include "dispatcher.h"
#include "toml.hpp"
#include <filesystem>
#include <unordered_map>

namespace poke {

    struct User {
        int wishes;
        std::vector<std::string> pokemon;
        time_t daily;
    };

    std::unordered_map<uint64_t, User> users;

    void Initialize() {
        std::filesystem::create_directory("poke");
        auto dirIterator = std::filesystem::directory_iterator("poke");

        for (const auto& entry : dirIterator) {
            std::ifstream file(entry.path());
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            auto table = toml::parse(content);

            User user;
            user.wishes = toml::get<int>(table["wishes"]);
            user.daily = toml::get<time_t>(table["daily"]);
            user.pokemon = toml::get<std::vector<std::string>>(table["pokemon"]);

            users[std::stoull(entry.path().filename().string())] = user;
        }
    }

    void CheckAndCreateUser(uint64_t nid)
    {
        std::string id = std::to_string(nid);

        if (users.find(std::stoull(id)) != users.end())
        {
            return;
        } else {
            User user;
            user.wishes = 5;
            user.daily = time(nullptr);
            user.pokemon = {};
            users[std::stoull(id)] = user;

            toml::table table;
            table["wishes"] = user.wishes;
            table["daily"] = user.daily;
            table["pokemon"] = user.pokemon;
            
            std::ofstream file(id + ".toml");
            file << toml::value(table);
        }
    }

    void Daily(const dpp::slashcommand_t& event)
    {}

    void Wishes(const dpp::slashcommand_t& event)
    {
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        event.reply("You have " + std::to_string(wishes) + " wishes.");
    }

    void Wish(const dpp::slashcommand_t& event)
    {
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        
        if (wishes == 0)
        {
            event.reply("You have no wishes left.");
        } else {
            int pokemon = (rand() % 1302) + 1;
            std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::to_string(pokemon) + ".gif";
            dpp::embed embed = dpp::embed()
                .set_image(url)
                .set_title("You got a #" + std::to_string(pokemon) + "!");
            event.reply(dpp::message(event.command.channel_id, embed));
        }
    }

}