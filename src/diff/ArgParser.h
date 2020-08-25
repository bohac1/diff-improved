#ifndef HEADER_DIFF_ARGPARSER
#define HEADER_DIFF_ARGPARSER
#include "constants/constants.h"
#include <cstring>
#include <string>
#include <map>
#include <memory>

namespace diff {

    /**
     * @brief class for parsing arguments passed with program call
     * 
     */
    class ArgParser {

        private:
            //! first input file
            std::string input1;
            //! second input file
            std::string input2;
            //! parsed map of input flags
            std::map<std::string, std::string> flags;
            
        public:
            /**
             * @brief Construct a new Arg Parser object
             * 
             * @param argc 
             * @param argv 
             */
            ArgParser(int argc, char **argv);

            /**
             * @brief Get first input file provided
             * 
             * @return std::string 
             */
            std::string getFristInput() const;
            
            /**
             * @brief Get the second input file provided
             * 
             * @return std::string 
             */
            std::string getSecondInput() const;
            
            /**
             * @brief writes internal state of ArgParser to string (for debugging purposes only)
             * 
             * @return std::string
             */
            std::string toString() const;

            /**
             * @brief Returns true if the input flag is present and writes its value to val, else returns false
             * 
             * @param search searched flag
             * @param val reference where the flag val will be stored
             * @return true flag exists
             * @return false flag does not exist
             */
            bool getFlag(std::string search, std::string & val) const;

    };
    
}

#endif
