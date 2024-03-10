#include "ai.hpp"
#include "restresults.h"
#include <liboai.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <dpp/dpp.h>

liboai::OpenAI oai;
bool initialized;
uint64_t lastImageTimestamp;

namespace artificial {
    
    void Initialize() {
        std::string token;
        std::ifstream keyFile("ai_key.txt");
        if (keyFile.is_open()) {
            std::getline(keyFile, token);
            keyFile.close();
        } else {
            std::cerr << "Failed to open AI token file" << std::endl;
            return;
        }

        bool success = oai.auth.SetKey(token);

        if (!success) {
            std::cerr << "Failed to set OpenAI API key" << std::endl;
            return;
        }

        initialized = true;
    }

    void GenerateImage(const dpp::slashcommand_t& event, const std::string& prompt) {
        if (!initialized) {
            event.reply(dpp::message("AI not initialized").set_flags(dpp::m_ephemeral));
            return;
        }

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp - lastImageTimestamp < 7200) {
            event.reply(dpp::message("You can only generate an image every 2 hours, or else Paris is going to go bankrupt. Next image available at <t:" + std::to_string(lastImageTimestamp + 7200) + "R>").set_flags(dpp::m_ephemeral));
            return;
        }

        lastImageTimestamp = timestamp;
        event.thinking(true, [event, prompt](const dpp::confirmation_callback_t& callback) {
            std::thread t([event, prompt] {
                liboai::Response res = oai.Image->create(
                    prompt,
                    1,
                    "256x256"
                );

                std::string url = res["data"][0]["url"];
                dpp::embed embed = dpp::embed()
                    .set_image(url)
                    .set_title("Your panda is ready!");
                event.edit_original_response(dpp::message(event.command.channel_id, embed));
            });
            t.detach();
        });
    }

}