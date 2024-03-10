#include "commands.hpp"
#include "admin.hpp"
#include "macros.hpp"
#include <toml.hpp>
#include "ai.hpp"

std::string boolToCheckbox(bool value) {
    return value ? "☑" : "☐";
}

unsigned constexpr str_hash(char const *input) {
    if (!*input)
        return 0;

    return *input ?
        static_cast<unsigned int>(*input) + 33 * str_hash(input + 1) :
        5381;
}

std::string currentPuzzleDescription;
std::string currentPuzzleSolution;

namespace commands {

    void Settings(dpp::cluster& bot, const dpp::slashcommand_t& event) {
        dpp::snowflake fileId = std::get<dpp::snowflake>(event.get_parameter("file"));
        dpp::attachment att = event.command.get_resolved_attachment(fileId);
        bot.request(att.url, dpp::http_method::m_get, [event](const dpp::http_request_completion_t& completion) {
            if (completion.status != 200) {
                event.reply(dpp::message("Failed to download file").set_flags(dpp::m_ephemeral));
                return;
            }

            if (completion.body.size() == 0) {
                event.reply(dpp::message("File size is 0").set_flags(dpp::m_ephemeral));
                return;
            }
            
            std::string rendererPretty = "unknown";
            std::string dsp = "unknown";
            bool isShaderJit = false;

            try {
                std::string toml(completion.body.begin(), completion.body.end());
                std::istringstream is(toml, std::ios_base::binary | std::ios_base::in);
                auto data = toml::parse(is);

                if (data.count("GPU") != 0) {
                    const auto& gpu = toml::find(data, "GPU");
                    const std::string& renderer = toml::find(gpu, "Renderer").as_string();
                    isShaderJit = toml::find(gpu, "EnableShaderJIT").as_boolean();
                    switch (str_hash(renderer.c_str())) {
                        case str_hash("vulkan"):
                            rendererPretty = "<:vulkant:1148648218658340894>";
                            break;
                        case str_hash("opengl"):
                            rendererPretty = "<:opengl:1148648565879603231>";
                            break;
                        default:
                            rendererPretty = "null";
                            break;
                    }
                }

                if (data.count("Audio") != 0) {
                    const auto& audio = toml::find(data, "Audio");
                    dsp = toml::find(audio, "DSPEmulation").as_string();
                }
            } catch (const std::exception& e) {
                event.reply(dpp::message("Failed to parse file").set_flags(dpp::m_ephemeral));
                return;
            }

            dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::sti_blue)
                .set_author(event.command.get_issuing_user().global_name + "'s settings", "", event.command.get_issuing_user().get_avatar_url())
                .set_thumbnail("https://panda3ds.com/images/panda-icon.png")
                .add_field("", "Renderer: " + rendererPretty)
                .add_field("", "Shader JIT: " + boolToCheckbox(isShaderJit))
                .add_field("", "DSP Emulation: " + dsp);

            dpp::message msg(event.command.channel_id, embed);
            event.reply(msg);
        });
    }

    void AddAdmin(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        dpp::guild_member member = event.command.get_resolved_member(std::get<dpp::snowflake>(event.get_parameter("user")));
        admins::AddAdmin(event, member);
    }

    void RemoveAdmin(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        dpp::guild_member member = event.command.get_resolved_member(std::get<dpp::snowflake>(event.get_parameter("user")));
        admins::RemoveAdmin(event, member);
    }

    void AddMacro(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));
        std::string response = std::get<std::string>(event.get_parameter("response"));

        if (name.find('\n') != std::string::npos || response.find('\n') != std::string::npos) {
            event.reply(dpp::message("Macro cannot contain '\n' as it's used as a separator internally").set_flags(dpp::m_ephemeral));
            return;
        }

        bool added = macros::AddMacro(name, response);
        event.reply(dpp::message(added ? "Macro added" : "Macro already exists").set_flags(dpp::m_ephemeral));
    }

    void AddMacroFile(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));
        dpp::snowflake fileId = std::get<dpp::snowflake>(event.get_parameter("file"));
        dpp::attachment att = event.command.get_resolved_attachment(fileId);
        bot.request(att.url, dpp::http_method::m_get, [event, name, att](const dpp::http_request_completion_t& completion) {
            if (completion.status != 200) {
                event.reply(dpp::message("Failed to download file").set_flags(dpp::m_ephemeral));
                return;
            }

            if (completion.body.size() == 0) {
                event.reply(dpp::message("File size is 0").set_flags(dpp::m_ephemeral));
                return;
            }

            std::ofstream file("macro_files/" + att.filename);
            file.write(completion.body.data(), completion.body.size());
            file.close();

            bool added = macros::AddMacro(name, "macro_files/" + att.filename);
            event.reply(dpp::message(added ? "Macro added" : "Macro already exists").set_flags(dpp::m_ephemeral));
        });
    }

    void RemoveMacro(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        std::string name = std::get<std::string>(event.get_parameter("name"));
        bool removed = macros::RemoveMacro(name);
        event.reply(dpp::message(removed ? "Macro removed" : "Macro does not exist").set_flags(dpp::m_ephemeral));
    }

    void Download(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        dpp::embed embed = dpp::embed()
            .set_color(dpp::colors::sti_blue)
            .set_author("Download", "", "https://panda3ds.com/images/panda-icon.png")
            .set_thumbnail("https://panda3ds.com/images/panda-icon.png")
            .set_description("Download the latest version of Panda3DS [here](https://panda3ds.com/download.html)");
        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void ClearCache(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        macros::ClearCache();
        event.reply(dpp::message("Cache cleared").set_flags(dpp::m_ephemeral));
    }

    void StartPuzzle(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        currentPuzzleSolution = std::get<std::string>(event.get_parameter("solution"));

        if (currentPuzzleSolution.find(' ') != std::string::npos) {
            event.reply(dpp::message("Puzzle solution cannot contain spaces, that would be too hard").set_flags(dpp::m_ephemeral));
            currentPuzzleSolution.clear();
            return;
        }

        currentPuzzleDescription = std::get<std::string>(event.get_parameter("description"));
        CurrentPuzzle(bot, event);
    }

    void SolvePuzzle(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        if (currentPuzzleSolution.empty()) {
            event.reply(dpp::message("No puzzle is currently active").set_flags(dpp::m_ephemeral));
            return;
        }

        if (std::get<std::string>(event.get_parameter("solution")) == currentPuzzleSolution) {
            bot.guild_member_add_role(event.command.guild_id, event.command.get_issuing_user().id, 1216474451282235402);
            event.reply(dpp::message("Congratulations! You've solved the puzzle and won the <@&1216474451282235402> role! The solution was: " + currentPuzzleSolution));
            currentPuzzleSolution.clear();
            currentPuzzleDescription.clear();
        } else {
            event.reply(dpp::message("Incorrect solution").set_flags(dpp::m_ephemeral));
        }
    }

    void CurrentPuzzle(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        if (currentPuzzleSolution.empty() && currentPuzzleDescription.empty()) {
            event.reply(dpp::message("No puzzle is currently active").set_flags(dpp::m_ephemeral));
            return;
        }

        dpp::embed embed = dpp::embed()
            .set_color(dpp::colors::sti_blue)
            .set_author(event.command.get_issuing_user().global_name + " has started a puzzle!", "", event.command.get_issuing_user().get_avatar_url())
            .set_thumbnail("https://panda3ds.com/images/panda-icon.png")
            .set_description("The first person to solve the puzzle wins the <@&1216474451282235402> role! The puzzle description is: " + currentPuzzleDescription)
            .add_field("Good luck!", "Use /solve_puzzle to solve the puzzle. The solution is a single word.");
        event.reply(dpp::message(event.command.channel_id, embed));
    }

    void Panda(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        artificial::GenerateImage(event, "A cute red panda!");
    }

    void AskPanda(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        event.reply(dpp::message("Just ping me in <#1118695733449723906> to ask your question!").set_flags(dpp::m_ephemeral));
    }

}