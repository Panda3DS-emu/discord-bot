#include "commands.hpp"
#include "admin.hpp"
#include "macros.hpp"
#include <dpp/dpp.h>
#include <toml.hpp>
#include <cpprest/http_client.h>

std::string boolToCheckbox(bool value) {
    return value ? "☑" : "☒";
}

unsigned constexpr str_hash(char const *input) {
    if (!*input)
        return 0;

    return *input ?
        static_cast<unsigned int>(*input) + 33 * str_hash(input + 1) :
        5381;
}

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
            .set_author("Latest Panda 3ds Release Build", "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
            .set_thumbnail("https://panda3ds.com/images/panda-icon.png")
        try {
        // Create an HTTP client
        web::http::client::http_client client(U("https://api.github.com"));

        // Make a GET request to fetch the latest release information
        web::http::http_request request(web::http::methods::GET);
        request.set_request_uri(U("/repos/wheremyfoodat/Panda3DS/releases/latest"));
        
        // Send the request asynchronously
        auto response = client.request(request).get();
        // Check if the response is successful
        if (response.status_code() == web::http::status_codes::OK) {
            // Parse the JSON response
            auto release = response.extract_json().get();

            // Access the release data
            auto assets = release[U("assets")].as_array();
            if (assets.empty()) {
                embed.set_description("No build attached to this release");
            } else {
                embed.set_title(release[U("name")].is_null() ? release[U("tag_name")].as_string() : release[U("name")].as_string())
                     .set_description(release[U("body")].as_string())
                     .set_url(release[U("html_url")].as_string())
                     .set_footer(release[U("tag_name")].as_string())
                     .set_timestamp(std::chrono::system_clock::now());

                embed.add_field("Release Build", "[Download APK](" + assets[0][U("browser_download_url")].as_string() + ")");
            }
        } else {
            // Handle unsuccessful response
            embed.set_description("No releases found on the repository");
        }

        // Send the embed message
        event.reply(dpp::message(event.command.channel_id, embed));
    } catch (const std::exception& e) {
        // Handle exceptions
        event.reply(dpp::message(event.command.channel_id, "An error occurred: " + std::string(e.what())));
    }
}

    void ClearCache(dpp::cluster &bot, const dpp::slashcommand_t &event) {
        macros::ClearCache();
        event.reply(dpp::message("Cache cleared").set_flags(dpp::m_ephemeral));
    }

}
