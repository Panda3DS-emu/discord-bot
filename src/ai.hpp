#pragma once

#include <dpp/dpp.h>
#include <string>

namespace artificial {
    void Initialize();
    void GenerateImage(const dpp::slashcommand_t& event, const std::string& prompt);
    void AskQuestion(const dpp::message_create_t& event, const std::string& question);
}