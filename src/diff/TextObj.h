#ifndef HEADER_DIFF_TEXTOBJ
#define HEADER_DIFF_TEXTOBJ
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>
#include "DiffObj.h"


namespace diff {

    class TextObj : public DiffObj  {

        private:

            /**
             * @brief Longest Common Subsequence algorithm
             * generates table of directions in the map whitch are then used to find shortest edit sequence
             *  
             * @param A first sequence of lines
             * @param B second sequence of lines
             * @return std::vector<std::vector<char>> table of directions
             */
            std::vector<std::vector<char>> LCSDiff(const std::vector<std::string> & A, const std::vector<std::string> & B) const;

            //! lines of text file
            std::vector<std::string> lines;
        public:

            /**
             * @brief Construct a new TextObj object from file path and argparser arguments
             * 
             * @param path path to file
             * @param ap argparser
             */
            TextObj(const std::filesystem::path & path, const ArgParser & ap);

            /**
             * @brief Construct a new blank TextObj object
             * useful for creating blank unique pointers
             */
            TextObj() = default;

            /**
             * @brief loads TextObj objects lines from input stream
             * 
             * @param is input stream
             */
            void loadFromIstream(std::istream & is);

            std::unique_ptr<DiffObj> diff(DiffObj & rhs, const ArgParser & ap) const override;

            void print(std::ostream & os, const ArgParser & ap) const override;
    };

}

#endif
