#pragma once

#include <string>
#include <dpp/dpp.h>

namespace macros {

    void Initialize();
    bool AddMacro(const std::string& name, const std::string& content);
    bool RemoveMacro(const std::string& name);
    void CheckForMacro(const dpp::message_create_t& event);

}