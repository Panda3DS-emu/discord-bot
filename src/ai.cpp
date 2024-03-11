#include "ai.hpp"
#include "restresults.h"
#include <liboai.h>
#include <fstream>
#include <iostream>
#include <functional>
#include <dpp/dpp.h>
#include <mutex>

liboai::OpenAI oai;
bool initialized;
uint64_t lastImageTimestamp = 0;
uint64_t lastQuestionTimestamp = 0;
liboai::Conversation convo;
liboai::Conversation convoFunny;
std::mutex mutex;
int lazyResetPromptCounter = 0;
std::string nonFunnyPrompt;

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

        std::ifstream promptFile("ai_prompt.txt");
        std::string data;
        promptFile.seekg(0, std::ios::end);
        data.resize(promptFile.tellg());
        promptFile.seekg(0, std::ios::beg);
        promptFile.read(&data[0], data.size());
        promptFile.close();
        nonFunnyPrompt = data;
        convo.SetSystemData("Your prompt is: " + data);

        std::ifstream funnyPromptFile("ai_funny_prompt.txt");
        std::string funnyData;
        funnyPromptFile.seekg(0, std::ios::end);
        funnyData.resize(funnyPromptFile.tellg());
        funnyPromptFile.seekg(0, std::ios::beg);
        funnyPromptFile.read(&funnyData[0], funnyData.size());
        funnyPromptFile.close();
        convoFunny.SetSystemData("Your prompt is: " + funnyData);

        initialized = true;
    }

    void GenerateImage(const dpp::slashcommand_t& event, const std::string& prompt) {
        if (!initialized) {
            event.reply(dpp::message("AI not initialized").set_flags(dpp::m_ephemeral));
            return;
        }

        uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (timestamp - lastImageTimestamp < 7200) {
            event.reply(dpp::message("You can only generate an image every 2 hours, or else Paris is going to go bankrupt. Next image available <t:" + std::to_string(lastImageTimestamp + 7200) + ":R>"));
            return;
        }

        lastImageTimestamp = timestamp;
        event.thinking(false, [event, prompt](const dpp::confirmation_callback_t& callback) {
            std::thread t([event, prompt] {
                liboai::Response res = oai.Image->create(
                    prompt,
                    1,
                    "256x256"
                );

                std::string url = res["data"][0]["url"];
                dpp::embed embed = dpp::embed()
                    .set_image(url)
                    .set_title("Your artificial panda is ready!");
                event.edit_original_response(dpp::message(event.command.channel_id, embed));
            });
            t.detach();
        });
    }

    void AskQuestion(const dpp::message_create_t& event, const std::string& prompt) {
        if (event.msg.channel_id != 1216695684665704479) {
            return;
        }

        if (!initialized) {
            event.reply(dpp::message("AI not initialized").set_flags(dpp::m_ephemeral));
            return;
        }

        std::thread t([event, prompt] {
            try {
                std::lock_guard<std::mutex> lock(mutex);
                uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                if (timestamp - lastQuestionTimestamp < 1) {
                    event.reply(dpp::message("You can only ask a question every 1 second, or else Paris is going to go bankrupt."));
                    return;
                }
                lastQuestionTimestamp = timestamp;

                srand(time(0));
                int randomness = rand() % 5;
                printf("randomness: %d\n", randomness);
                if (randomness == 0) {
                    convoFunny.AddUserData("HUMAN: " + prompt + "\n");
                    liboai::Response response = oai.ChatCompletion->create(
                        "gpt-3.5-turbo", convoFunny
                    );
                    convoFunny.Update(response);
                    event.reply(dpp::message(convoFunny.GetLastResponse()));
                } else {
                    lazyResetPromptCounter++;
                    if (lazyResetPromptCounter > 20) {
                        convo = liboai::Conversation();
                        convo.SetSystemData("Your prompt is: " + nonFunnyPrompt);
                    }
                    convo.AddUserData("HUMAN: " + prompt + "\n");
                    liboai::Response response = oai.ChatCompletion->create(
                        "gpt-3.5-turbo", convo
                    );
                    convo.Update(response);
                    event.reply(dpp::message(convo.GetLastResponse()));
                }
            } catch (std::exception& e) {
                event.reply(dpp::message("This message crashed the AI lmao"));
                convo.PopUserData();
            }
        });
        t.detach();
    }

}