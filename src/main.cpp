#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <fstream>
#include "diff/ArgParser.h"
#include "diff/DiffObj.h"
#include "diff/constants/constants.h"

int main(int argc, char **argv) {
    using diff::ArgParser;
    using diff::DiffObj;

    try {
        // parse arguments
        ArgParser ap = ArgParser(argc, argv);

        // set recursion depth
        std::string tmp;
        int recursionDepth = -1;
        if(ap.getFlag(diff::constants::FLAG_DEPTH, tmp)) {
            try {
                recursionDepth = std::stoi(tmp);
            } catch(std::invalid_argument & e) {
                std::cerr << diff::constants::RTEX_INVALID_FLAG_OPTION << std::endl;
                return 1;
            }
        }
        
        // based on provided arguments and extensions of files creates instance of DiffObj 
        auto a = DiffObj::createByMimeType(ap.getFristInput(), ap, recursionDepth);
        auto b = DiffObj::createByMimeType(ap.getSecondInput(), ap, recursionDepth);
        std::unique_ptr<DiffObj> c;
        try {
            // diff second DiffObj from the first one (create new object that holds the result)
            c = std::move(a->diff(*b, ap));
        } catch(std::bad_cast & e) {
            std::cerr << diff::constants::RTEX_DIFF_FILES_ARE_NOT_OF_THE_SAME_TYPE << std::endl;
            return 1;
        }
        // print to outupt based on args and object type
        std::string outFile;
        if(ap.getFlag(diff::constants::FLAG_OUT, outFile)) {
            // write to file
            std::ofstream of{ outFile, std::ios_base::binary };
            if(of) {
                c->print(of, ap);
                if(!of.good()) {
                    throw std::runtime_error(diff::constants::RTEX_IFSTREAM_OPEN_ERR);
                }
            } else {
                throw std::runtime_error(diff::constants::RTEX_IFSTREAM_OPEN_ERR);
            }
        } else {
            // write to console
            c->print(std::cout, ap);
            if(!std::cout.good()) {
                throw std::runtime_error(diff::constants::RTEX_IFSTREAM_OPEN_ERR);
            }
        }
    } catch(std::runtime_error & e) {
        // catch runtime errors and write them to cerr stream
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
