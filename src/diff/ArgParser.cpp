#include <stdexcept>
#include "ArgParser.h"

namespace diff {

    ArgParser::ArgParser(int argc, char **argv) {
        if(argc < 3) {
            throw std::runtime_error(constants::RTEX_INVALID_ARG_COUNT);
        }

        input1 = std::string(argv[1]);
        input2 = std::string(argv[2]);
        
        for(int i=3; i < argc; ++i) {
            // parse and load vals to arg map
            std::string in = std::string(argv[i]);
            std::size_t valpos = in.find("=");
        
            if(valpos != std::string::npos) {
                auto it = constants::FLAG_DEF.find(in.substr(0,valpos));
                if(it != constants::FLAG_DEF.end()) {
                    if(it->second) {
                        flags.insert(std::pair<std::string, std::string>(in.substr(0,valpos), in.substr(valpos+1)));
                        continue;
                    }
                }
            } else {
                // key only
                auto it = constants::FLAG_DEF.find(in);
                if(it != constants::FLAG_DEF.end()) {
                    if(!it->second) {
                        flags.insert(std::pair<std::string, std::string>(in, std::string("")));
                        continue;
                    }
                }
            }
        
            throw std::runtime_error(constants::RTEX_INVALID_FLAG);
        }
    }
    
    std::string ArgParser::getFristInput() const {
        return input1;
    }
    
    std::string ArgParser::getSecondInput() const {
        return input2;
    }

    std::string ArgParser::toString() const {
        std::string out = "";
        out += "if1: " + input1 + "\n";
        out += "if2: " + input2 + "\n";

        for(auto const& it : flags) {
            out += it.first + " = " + it.second + "\n";
        }

        return out;
    }

    bool ArgParser::getFlag(std::string search, std::string & val) const {
        auto it = flags.find(search);
        if(it != flags.end()) {
            val = it->second;
            return true;
        }
        
        return false;
    }

}
