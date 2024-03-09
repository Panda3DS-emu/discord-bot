#include "macros.hpp"
#include "colors.h"
#include <unordered_map>

namespace macros {

    std::unordered_map<std::string, std::string> macros;

    void saveMacros() {
        std::ofstream macrosFile("macros.txt");
        if (macrosFile.is_open()) {
            for (const auto& macro : macros) {
                macrosFile << macro.first << "\n" << macro.second << std::endl;
            }
        }
    }

    void Initialize() {
        std::ifstream macrosFile("macros.txt");
        if (macrosFile.is_open()) {
            int lines = std::count(std::istreambuf_iterator<char>(macrosFile), std::istreambuf_iterator<char>(), '\n');
            macrosFile.seekg(0, std::ios::beg);
            if (lines % 2 != 0) {
                std::cerr << "Invalid macros file" << std::endl;
                return;
            }

            std::string name;
            std::string content;
            while (std::getline(macrosFile, name)) {
                std::getline(macrosFile, content);
                macros[name] = content;
            }
        }
    }

    bool AddMacro(const std::string &name, const std::string &content) {
        if (macros.find(name) != macros.end())
            return false;
        macros[name] = content;
        saveMacros();
        return true;
    }

    bool RemoveMacro(const std::string &name) {
        if (macros.find(name) == macros.end())
            return false;
        macros.erase(name);
        saveMacros();
        return true;
    }

    void CheckForMacro(const dpp::message_create_t& event) {
        std::string message = event.msg.content;
        auto id = event.msg.message_reference.message_id;
        if (macros.find(message) != macros.end()) {
            dpp::embed embed = dpp::embed().set_description(macros[message]).set_color(dpp::colors::red_fox);
            dpp::message msg(event.msg.channel_id, embed);

            if (!event.msg.message_reference.message_id.empty())
                msg.set_reference(event.msg.message_reference.message_id);

            event.send(msg);
        }
    }

}