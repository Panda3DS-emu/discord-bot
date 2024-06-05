#include "poke.hpp"
#include "colors.h"
#include "dispatcher.h"
#include "toml.hpp"
#include <filesystem>
#include <mutex>
#include <unordered_map>

namespace poke {

    std::vector<int> legendaries = {
        144, 145, 146, 150, 243, 244, 245, 249, 250, 251, 
        377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 
        480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 
        490, 491, 492, 493, 638, 639, 640, 641, 642, 643, 
        644, 645, 646, 647, 648, 649, 716, 717, 718, 719, 
        720, 721, 785, 786, 787, 788, 789, 790, 791, 792, 
        800, 801, 802, 807, 888, 889, 890, 891, 892, 894, 
        895, 896, 897, 898
    };
    
    time_t lastBanner;
    int bannerPokemon;

    struct User {
        int wishes;
        std::vector<std::string> pokemon;
        time_t daily;
        int pity;
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

        if (std::filesystem::exists("banner.toml")) {
            auto table = toml::parse("banner.toml");
            lastBanner = toml::get<time_t>(table["lastBanner"]);
            bannerPokemon = toml::get<int>(table["bannerPokemon"]);
        } else {
            lastBanner = time(nullptr);
            bannerPokemon = legendaries[rand() % legendaries.size()];
        }
    }

    void CheckAndCreateUser(uint64_t nid)
    {
        if (time(nullptr) - lastBanner > 259200)
        {
            lastBanner = time(nullptr);
            bannerPokemon = legendaries[rand() % legendaries.size()];

            toml::table table;
            table["lastBanner"] = lastBanner;
            table["bannerPokemon"] = bannerPokemon;

            std::ofstream file("banner.toml");
            file << toml::value(table);
        }

        std::string id = std::to_string(nid);

        if (users.find(std::stoull(id)) != users.end())
        {
            return;
        } else {
            User user;
            user.wishes = 5;
            user.daily = time(nullptr);
            user.pokemon = {};
            user.pity = 0;
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
            int roll1 = (rand() % 920) + 1;
            int roll2 = (rand() % 920) + 1;
            int roll;

            if (roll1 == bannerPokemon || roll2 == bannerPokemon)
            {
                roll = bannerPokemon;
            } else {
                roll = roll1;

                // Can't roll legendaries that are not the banner
                while (std::find(legendaries.begin(), legendaries.end(), roll) != legendaries.end())
                {
                    roll = (rand() % 920) + 1;
                }
            }

            if (users[id].pity >= 50)
            {
                roll = bannerPokemon;
            }
            
            std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::to_string(roll) + ".gif";
            dpp::embed embed = dpp::embed()
                .set_image(url);

            users[id].wishes--;

            if (roll == bannerPokemon)
            {
                embed.set_title("WOAH! You got #" + std::to_string(roll) + "!");
                embed.set_color(dpp::colors::orange);
            } else {
                embed.set_title("You got #" + std::to_string(roll) + ".");
                embed.set_color(dpp::colors::green);
            }

            if (std::find(users[id].pokemon.begin(), users[id].pokemon.end(), std::to_string(roll)) == users[id].pokemon.end())
            {
                users[id].pokemon.push_back(std::to_string(roll));
            } else {
                embed.set_description("You already have this pokemon so it decomposed into 1 wish.");
            }

            if (roll == bannerPokemon)
            {
                users[id].pity = 0;
            } else {
                users[id].pity++;
            }

            event.reply(dpp::message(event.command.channel_id, embed));

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

    void Banner(const dpp::slashcommand_t& event)
    {
        std::lock_guard<std::mutex> lock(mtx);
        uint64_t id = event.command.get_issuing_user().id;
        CheckAndCreateUser(id);

        std::string url = "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/showdown/" + std::to_string(bannerPokemon) + ".gif";

        dpp::embed embed = dpp::embed()
            .set_image(url)
            .set_title("Today's banner is #" + std::to_string(bannerPokemon) + "!")
            .set_description("This pokemon has double chance to appear when you wish and you are guaranteed to get it after 50 wishes. Next banner reset: <t:" + std::to_string(lastBanner + 259200) + ":R>. Wishes left until banner pull: " + std::to_string(50 - users[id].pity))
            .set_color(dpp::colors::purple);
        
        event.reply(dpp::message(event.command.channel_id, embed));
    }

}