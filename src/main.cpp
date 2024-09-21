#include <dpp/dpp.h>
#include "admin.hpp"
#include "appcommand.h"
#include "commands.hpp"
#include "dispatcher.h"
#include "macros.hpp"
#include "ai.hpp"
#include "poke.hpp"
#include "restresults.h"

constexpr bool clearCommands = false;

struct Command {
    std::string name;
    std::string description;
    commands::CommandFunction function;
    bool adminOnly;
    std::vector<dpp::command_option> options;
};

Command botCommands[] = {
    {
        .name = "settings",
        .description = "Upload your Panda3DS settings file in a nicely formatted manner",
        .function = commands::Settings,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_attachment, "file", "The Panda3DS settings file", true)
        }
    },
    {
        .name = "add_admin",
        .description = "Add a user to the list of admins",
        .function = commands::AddAdmin,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_user, "user", "The user to add as an admin", true)
        }
    },
    {
        .name = "remove_admin",
        .description = "Remove a user from the list of admins",
        .function = commands::RemoveAdmin,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_user, "user", "The user to remove as an admin", true)
        }
    },
    {
        .name = "add_macro",
        .description = "Add a macro to the list of macros",
        .function = commands::AddMacro,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "name", "The name of the macro", true),
            dpp::command_option(dpp::co_string, "response", "The response of the macro", true)
        }
    },
    {
        .name = "add_macro_file",
        .description = "Add a macro that responds with a file to the list of macros",
        .function = commands::AddMacroFile,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "name", "The name of the macro", true),
            dpp::command_option(dpp::co_attachment, "file", "The file to respond with", true)
        }
    },
    {
        .name = "remove_macro",
        .description = "Remove a macro from the list of macros",
        .function = commands::RemoveMacro,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "name", "The name of the macro", true)
        }
    },
    {
        .name = "download",
        .description = "Show download links for Panda3DS",
        .function = commands::Download,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "clear_cache",
        .description = "Clear the macro image cache",
        .function = commands::ClearCache,
        .adminOnly = true,
        .options = {}
    },
    {
        .name = "start_puzzle",
        .description = "Start a puzzle",
        .function = commands::StartPuzzle,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "description", "The description of the puzzle", true),
            dpp::command_option(dpp::co_string, "solution", "The solution of the puzzle", true)
        }
    },
    {
        .name = "solve_puzzle",
        .description = "Solve a puzzle",
        .function = commands::SolvePuzzle,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_string, "solution", "The solution of the puzzle", true)
        }
    },
    {
        .name = "current_puzzle",
        .description = "Show the current puzzle",
        .function = commands::CurrentPuzzle,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "panda",
        .description = "Show an artificial panda!",
        .function = commands::Panda,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "ask_panda",
        .description = "Ask the artificial panda a question!",
        .function = commands::AskPanda,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_string, "question", "The question to ask the panda", true)
        }
    },
    {
        .name = "set_prompt",
        .description = "Set the prompt for the AI",
        .function = commands::SetPrompt,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "prompt", "The prompt for the AI", true)
        }
    },
    {
        .name = "reset_prompt",
        .description = "Reset the prompt for the AI",
        .function = commands::ResetPrompt,
        .adminOnly = true,
        .options = {}
    },
    {
        .name = "hidden_say",
        .description = "Make the bot say something",
        .function = commands::HiddenSay,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_string, "message", "The message to say", true),
            dpp::command_option(dpp::co_string, "message_id", "The message id to reply to", false),
        }
    },
    {
        .name = "hidden_file",
        .description = "Make the bot send a file",
        .function = commands::HiddenFile,
        .adminOnly = true,
        .options = {
            dpp::command_option(dpp::co_attachment, "file", "The file to send", true),
            dpp::command_option(dpp::co_string, "message_id", "The message id to reply to", false),
        }
    },
    {
        .name = "history",
        .description = "Show the hidden_say history of the bot",
        .function = commands::History,
        .adminOnly = true,
        .options = {}
    },
    {
        .name = "image",
        .description = "Generate an image from the AI",
        .function = commands::Image,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_string, "prompt", "The prompt for the AI", true)
        }
    },
    {
        .name = "clear_context",
        .description = "Clear the context of the AI",
        .function = commands::ClearContext,
        .adminOnly = true,
        .options = {}
    },
    {
        .name = "wishes",
        .description = "See how many wishes you have",
        .function = commands::Wishes,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "wish",
        .description = "Make a wish",
        .function = commands::Wish,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "multiwish",
        .description = "Make many wishes",
        .function = commands::MultiWish,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_integer, "wishes", "The number of wishes", true),
        }
    },
    {
        .name = "daily",
        .description = "Get your daily wishes",
        .function = commands::Daily,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "list",
        .description = "List your Pokemon",
        .function = commands::List,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "favorite",
        .description = "Favorite a Pokemon",
        .function = commands::Favorite,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_string, "pokemon", "The Pokemon to favorite", true)
        }
    },
    {
        .name = "banner",
        .description = "Show the current legendary banner",
        .function = commands::Banner,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "leaderboard",
        .description = "Show the leaderboard",
        .function = commands::Leaderboard,
        .adminOnly = false,
        .options = {}
    },
    {
        .name = "roulette",
        .description = "Bet wishes on the roulette. 18 red slots and 18 black slots and 1 green slot",
        .function = commands::Roulette,
        .adminOnly = false,
        .options = {
            dpp::command_option(dpp::co_integer, "wishes_on_red", "The number of wishes to bet on red", true),
            dpp::command_option(dpp::co_integer, "wishes_on_black", "The number of wishes to bet on black", true),
        }
    }
};

int main() {
    admins::Initialize();
    macros::Initialize();
    artificial::Initialize();
    poke::Initialize();

    std::string token;
    std::ifstream tokenFile("token.txt");
    if (tokenFile.is_open()) {
        std::getline(tokenFile, token);
        tokenFile.close();
    } else {
        std::cerr << "Failed to open token file" << std::endl;
        return 1;
    }

    dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        std::string name = event.command.get_command_name();
        Command* command = std::find_if(std::begin(botCommands), std::end(botCommands), [&name](const Command& command) {
            return command.name == name;
        });

        if (command != std::end(botCommands)) {
            if (command->adminOnly) {
                admins::AdminCommand(command->function, bot, event);
            } else {
                command->function(bot, event);
            }
        } else {
            std::cerr << "Unknown command: " << name << std::endl;
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (clearCommands && dpp::run_once<struct clear_bot_commands>()) {
            bot.global_bulk_command_delete();
        }

        if (dpp::run_once<struct register_bot_commands>()) {
            for (const Command& command : botCommands) {
                dpp::slashcommand slashCommand(command.name, command.description, bot.me.id);
                for (const dpp::command_option& option : command.options) {
                    slashCommand.add_option(option);
                }
                bot.global_command_create(slashCommand);
            }
        }
    });

    bot.on_button_click([](const dpp::button_click_t& event) {
        std::string id = event.custom_id;

        if (id.find("page_next_") == 0) {
        }
    });

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg.author.is_bot()) {
            return;
        }

        if (event.msg.mentions.size() == 1 && event.msg.mentions[0].first.id == bot.me.id) {
            artificial::AskQuestion(event, event.msg.author.global_name + ": " + event.msg.content);
        } else {
            macros::CheckForMacro(event);
        }
    });
    
    bot.start(dpp::st_wait);
}
