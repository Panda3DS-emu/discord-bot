#include "ai.hpp"
#include <liboai.h>

liboai::OpenAI oai;
bool initialized;

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

        liboai::Response res = oai.Image->create(
            "A snake in the grass!",
            1,
            "256x256"
        );

        std::cout << res["data"][0]["url"] << std::endl;
        
        initialized = true;
    }

}