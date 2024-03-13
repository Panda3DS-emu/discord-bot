#include <dpp/dpp.h>
#include "admin.hpp"
#include "appcommand.h"
#include "commands.hpp"
#include "macros.hpp"
#include "ai.hpp"

constexpr bool clearCommands = false;

int main() {
    admins::Initialize();
    macros::Initialize();
    artificial::Initialize();

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
        // I cba to use a neater way rn but TODO refactor this
        if (name == "settings") {
            commands::Settings(bot, event);
        } else if (name == "add_admin") {
            admins::AdminCommand(commands::AddAdmin, bot, event);
        } else if (name == "remove_admin") {
            admins::AdminCommand(commands::RemoveAdmin, bot, event);
        } else if (name == "add_macro") {
            admins::AdminCommand(commands::AddMacro, bot, event);
        } else if (name == "remove_macro") {
            admins::AdminCommand(commands::RemoveMacro, bot, event);
        } else if (name == "download") {
            commands::Download(bot, event);
        } else if (name == "add_macro_file") {
            admins::AdminCommand(commands::AddMacroFile, bot, event);
        } else if (name == "clear_cache") {
            admins::AdminCommand(commands::ClearCache, bot, event);
        } else if (name == "start_puzzle") {
            admins::AdminCommand(commands::StartPuzzle, bot, event);
        } else if (name == "solve_puzzle") {
            commands::SolvePuzzle(bot, event);
        } else if (name == "current_puzzle") {
            commands::CurrentPuzzle(bot, event);
        } else if (name == "panda") {
            commands::Panda(bot, event);
        } else if (name == "ask_panda") {
            commands::AskPanda(bot, event);
        } else if (name == "set_prompt") {
            admins::AdminCommand(commands::SetPrompt, bot, event);
        } else if (name == "reset_prompt") {
            admins::AdminCommand(commands::ResetPrompt, bot, event);
        } else if (name == "hidden_say") {
            admins::AdminCommand(commands::HiddenSay, bot, event);
        } else if (name == "hidden_file") {
            admins::AdminCommand(commands::HiddenFile, bot, event);
        } else if (name == "history") {
            commands::History(bot, event);
        } else if (name == "image") {
            admins::AdminCommand(commands::Image, bot, event);
        }
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (clearCommands && dpp::run_once<struct clear_bot_commands>()) {
            bot.global_bulk_command_delete();
        }

        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand logFileCommand("settings", "Upload your Panda3DS settings file in a nicely formatted manner", bot.me.id);
            logFileCommand.add_option(
                dpp::command_option(dpp::co_attachment, "file", "The Panda3DS settings file", true)
            );
    
            dpp::slashcommand addAdminCommand("add_admin", "Add a user to the list of admins", bot.me.id);
            addAdminCommand.add_option(
                dpp::command_option(dpp::co_user, "user", "The user to add as an admin", true)
            );

            dpp::slashcommand removeAdminCommand("remove_admin", "Remove a user from the list of admins", bot.me.id);
            removeAdminCommand.add_option(
                dpp::command_option(dpp::co_user, "user", "The user to remove as an admin", true)
            );

            dpp::slashcommand addMacroCommand("add_macro", "Add a macro to the list of macros", bot.me.id);
            addMacroCommand.add_option(
                dpp::command_option(dpp::co_string, "name", "The name of the macro", true)
            ).add_option(
                dpp::command_option(dpp::co_string, "response", "The response of the macro", true)
            );

            dpp::slashcommand addMacroFileCommand("add_macro_file", "Add a macro that responds with a file to the list of macros", bot.me.id);
            addMacroFileCommand.add_option(
                dpp::command_option(dpp::co_string, "name", "The name of the macro", true)
            ).add_option(
                dpp::command_option(dpp::co_attachment, "file", "The file to respond with", true)
            );

            dpp::slashcommand removeMacroCommand("remove_macro", "Remove a macro from the list of macros", bot.me.id);
            removeMacroCommand.add_option(
                dpp::command_option(dpp::co_string, "name", "The name of the macro", true)
            );

            dpp::slashcommand downloadCommand("download", "Show download links for Panda3DS", bot.me.id);

            dpp::slashcommand clearCacheCommand("clear_cache", "Clear the image cache", bot.me.id);

            dpp::slashcommand startPuzzleCommand("start_puzzle", "Start a puzzle", bot.me.id);
            startPuzzleCommand.add_option(
                dpp::command_option(dpp::co_string, "description", "The description of the puzzle", true)
            ).add_option(
                dpp::command_option(dpp::co_string, "solution", "The solution of the puzzle", true)
            );

            dpp::slashcommand solvePuzzleCommand("solve_puzzle", "Solve a puzzle", bot.me.id);
            solvePuzzleCommand.add_option(
                dpp::command_option(dpp::co_string, "solution", "The solution of the puzzle", true)
            );

            dpp::slashcommand currentPuzzleCommand("current_puzzle", "Show the current puzzle", bot.me.id);

            dpp::slashcommand pandaCommand("panda", "Show an artificial panda!", bot.me.id);

            dpp::slashcommand askPandaCommand("ask_panda", "Ask the artificial panda a question!", bot.me.id);
            askPandaCommand.add_option(
                dpp::command_option(dpp::co_string, "question", "The question to ask the panda", true)
            );

            dpp::slashcommand setPromptCommand("set_prompt", "Set the prompt for the AI", bot.me.id);
            setPromptCommand.add_option(
                dpp::command_option(dpp::co_string, "prompt", "The prompt for the AI", true)
            );

            dpp::slashcommand resetPromptCommand("reset_prompt", "Reset the prompt for the AI", bot.me.id);

            dpp::slashcommand hiddenSayCommand("hidden_say", "Make the bot say something", bot.me.id);
            hiddenSayCommand.add_option(
                dpp::command_option(dpp::co_string, "message", "The message to say", true)
            );

            dpp::slashcommand hiddenFileCommand("hidden_file", "Make the bot send a file", bot.me.id);
            hiddenFileCommand.add_option(
                dpp::command_option(dpp::co_attachment, "file", "The file to send", true)
            );

            dpp::slashcommand historyCommand("history", "Show the hidden_say history of the bot", bot.me.id);

            dpp::slashcommand imageCommand("image", "Generate an image from the AI", bot.me.id);
            imageCommand.add_option(
                dpp::command_option(dpp::co_string, "prompt", "The prompt for the AI", true)
            );

            bot.global_command_create(logFileCommand);
            bot.global_command_create(addAdminCommand);
            bot.global_command_create(removeAdminCommand);
            bot.global_command_create(addMacroCommand);
            bot.global_command_create(addMacroFileCommand);
            bot.global_command_create(removeMacroCommand);
            bot.global_command_create(downloadCommand);
            bot.global_command_create(clearCacheCommand);
            bot.global_command_create(startPuzzleCommand);
            bot.global_command_create(solvePuzzleCommand);
            bot.global_command_create(currentPuzzleCommand);
            bot.global_command_create(pandaCommand);
            bot.global_command_create(askPandaCommand);
            bot.global_command_create(setPromptCommand);
            bot.global_command_create(resetPromptCommand);
            bot.global_command_create(hiddenSayCommand);
            bot.global_command_create(hiddenFileCommand);
            bot.global_command_create(historyCommand);
            bot.global_command_create(imageCommand);
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
