#include "poke.hpp"
#include "dispatcher.h"
#include "toml.hpp"
#include <filesystem>
#include <mutex>
#include <unordered_map>

namespace poke {

    struct User {
        int wishes;
        std::vector<std::string> pokemon;
        time_t daily;
    };

    std::unordered_map<uint64_t, User> users;

    std::mutex mtx;

    std::string getPath(uint64_t id)
    {
        return "poke/" + std::to_string(id) + ".toml";
    }

    void Initialize() {
        srand(time(nullptr));
        std::filesystem::create_directory("poke");
        auto dirIterator = std::filesystem::directory_iterator("poke");

        for (const auto& entry : dirIterator) {
            auto table = toml::parse(entry.path().string());

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

            std::ofstream file(getPath(nid));
            file << toml::value(table);
        }
    }

    void Daily(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        time_t now = time(nullptr);
        time_t daily = users[id].daily;

        if (now - daily < 86400)
        {
            event.reply("You already claimed your daily reward.");
        } else {
            users[id].daily = now;
            users[id].wishes += 5;

            std::ofstream file(getPath(id));
            toml::table table;
            table["wishes"] = users[id].wishes;
            table["daily"] = users[id].daily;
            table["pokemon"] = users[id].pokemon;
            file << toml::value(table);

            event.reply("You claimed your daily reward. You now have " + std::to_string(users[id].wishes) + " wishes.");
        }
    }

    void Wishes(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        event.reply("You have " + std::to_string(wishes) + " wishes.");
    }

    void Wish(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        int wishes = users[id].wishes;
        
        if (wishes == 0)
        {
            event.reply("You have no wishes left.");
        } else {
            int pokemon = (rand() % 920) + 1;
            std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::to_string(pokemon) + ".gif";
            dpp::embed embed = dpp::embed()
                .set_image(url)
                .set_title("You got a #" + std::to_string(pokemon) + "!");
            event.reply(dpp::message(event.command.channel_id, embed));

            users[id].wishes--;
            users[id].pokemon.push_back(std::to_string(pokemon));

            std::ofstream file(getPath(id));
            toml::table table;
            table["wishes"] = users[id].wishes;
            table["daily"] = users[id].daily;
            table["pokemon"] = users[id].pokemon;
            file << toml::value(table);
        }
    }

    void List(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        dpp::embed embed = dpp::embed()
            .set_title("You have " + std::to_string(users[id].pokemon.size()) + " Pokemon.")
            .set_color(0x00FF00);

        if (users[id].pokemon.size() > 0)
            embed.set_thumbnail("https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + users[id].pokemon[0] + ".gif");

        std::string description = "";
        for (std::string pokemon : users[id].pokemon)
        {
            description += "#" + pokemon + "\n";
        }

        embed.set_description(description);

        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void Favorite(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        std::string pokemon = std::get<std::string>(event.get_parameter("pokemon"));

        if (std::find(users[id].pokemon.begin(), users[id].pokemon.end(), pokemon) == users[id].pokemon.end())
        {
            event.reply("You don't have that Pokemon.");
        } else {
            users[id].pokemon.erase(std::remove(users[id].pokemon.begin(), users[id].pokemon.end(), pokemon), users[id].pokemon.end());
            users[id].pokemon.insert(users[id].pokemon.begin(), pokemon);

            std::ofstream file(getPath(id));
            toml::table table;
            table["wishes"] = users[id].wishes;
            table["daily"] = users[id].daily;
            table["pokemon"] = users[id].pokemon;
            file << toml::value(table);

            event.reply("You favorited " + pokemon + ".");
        }
    }

}