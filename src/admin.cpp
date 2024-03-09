#include "admin.hpp"

namespace admins {

    std::vector<std::string> adminIds = {
        "540912691888259133", // Peach's ID
        "1136439819287928963", // Paris' ID
    };

    void loadExtraAdmins() {
        std::ifstream adminsFile("admins.txt");
        if (adminsFile.is_open()) {
            std::string line;
            while (std::getline(adminsFile, line)) {
                if (!line.empty())
                    adminIds.push_back(line);
            }
        } else {
            std::cerr << "Failed to open admins file" << std::endl;
            return;
        }
    }

    void AdminCommand(const commands::CommandFunction& command, dpp::cluster& bot, const dpp::slashcommand_t& event) {
        if (std::find(adminIds.begin(), adminIds.end(), event.command.get_issuing_user().id.str()) != adminIds.end()) {
            command(bot, event);
        } else {
            event.reply(dpp::message("You do not have permission to use this command").set_flags(dpp::m_ephemeral));
        }
    }

    void Initialize() {
        loadExtraAdmins();
    }

    void AddAdmin(const dpp::slashcommand_t& event, const dpp::guild_member& member) {
        std::string id = member.user_id.str();
        if (std::find(adminIds.begin(), adminIds.end(), id) == adminIds.end()) {
            adminIds.push_back(id);
            std::ofstream adminsFile("admins.txt");
            if (adminsFile.is_open()) {
                for (const std::string& adminId : adminIds) {
                    adminsFile << adminId << std::endl;
                }
            }
            event.reply(dpp::message("User <@" + member.user_id.str() + "> added as an admin").set_flags(dpp::m_ephemeral));
        } else {
            event.reply(dpp::message("User <@" + member.user_id.str() + "> is already an admin").set_flags(dpp::m_ephemeral));
        }
    }
    
    void RemoveAdmin(const dpp::slashcommand_t& event, const dpp::guild_member& member) {
        std::string id = member.user_id.str();

        if (id == "540912691888259133" || id == "1136439819287928963") {
            event.reply(dpp::message("You silly goose, you can't remove the bot's creators from admin").set_flags(dpp::m_ephemeral));
            return;
        }

        auto it = std::find(adminIds.begin(), adminIds.end(), id);
        if (it != adminIds.end()) {
            adminIds.erase(it);
            std::ofstream adminsFile("admins.txt");
            if (adminsFile.is_open()) {
                for (const std::string& adminId : adminIds) {
                    adminsFile << adminId << std::endl;
                }
            }
            event.reply(dpp::message("User <@" + member.user_id.str() + "> removed as an admin").set_flags(dpp::m_ephemeral));
        } else {
            event.reply(dpp::message("User <@" + member.user_id.str() + "> is not an admin").set_flags(dpp::m_ephemeral));
        }
    }

}