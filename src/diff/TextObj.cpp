#include "TextObj.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>


namespace diff {

    TextObj::TextObj(const std::filesystem::path & path, const ArgParser & ap) {
        fileName = path.filename();
        std::ifstream is(path);
        if(is) {
            std::string tmp;
            bool ignoreEmpty = ap.getFlag(constants::FLAG_IGNORE_EMPTY_LINE, tmp);
            std::string line;
            while(std::getline(is, line)) {
                if(!(line == "" && ignoreEmpty))
                    lines.push_back(line);
            }
            if(is.bad() && !is.eof()) {
                throw std::runtime_error(constants::RTEX_FSTREAM_READ_ERR);
            }
        } else {
            throw std::runtime_error(constants::RTEX_FSTREAM_OPEN_ERR);
        }
    }

    void TextObj::loadFromIstream(std::istream & is) {
        if(is) {
            std::string line;
            while(std::getline(is, line)) {
                lines.push_back(line);
            }
        } else {
            throw std::runtime_error(constants::RTEX_FSTREAM_OPEN_ERR);
        }
    }

    std::unique_ptr<DiffObj> TextObj::diff(DiffObj & rhs, const ArgParser & ) const {
        TextObj & trhs = dynamic_cast<TextObj &>(rhs);
        auto ptr = std::make_unique<TextObj>();
        std::vector<std::vector<char>> directions = LCSDiff(lines, trhs.lines);

        size_t i = lines.size();
        size_t j = trhs.lines.size();
        while(i > 0 || j > 0) {
            switch (directions[i][j]) {
            case 'D':
                ptr->lines.push_back("" + lines[i - 1]);
                --i;
                --j;
                break;
            case 'U':
                ptr->lines.push_back(constants::TEXTOBJ_OPERATION_ADD + lines[i - 1]);
                --i;
                break;
            case 'L':
                ptr->lines.push_back(constants::TEXTOBJ_OPERATION_REM + trhs.lines[j - 1]);
                --j;
                break;
            case 'E':
            default:
                if(j == 0) {
                    ptr->lines.push_back(constants::TEXTOBJ_OPERATION_ADD + lines[i - 1]);
                    --i;
                } else {
                    ptr->lines.push_back(constants::TEXTOBJ_OPERATION_REM + trhs.lines[j - 1]);
                    --j;
                }
                break;
            }
        }

        std::reverse(ptr->lines.begin(),ptr->lines.end());
        return ptr;
    }

    void TextObj::print(std::ostream & os, const ArgParser & ) const {
        std::string outFormat;
        for(const std::string & line : lines) {
            os << line << std::endl;
        }
    }

    std::vector<std::vector<char>> TextObj::LCSDiff(const std::vector<std::string> & A, const std::vector<std::string> & B) const{
        size_t sizeA = A.size();
        size_t sizeB = B.size();
        std::vector<std::vector<int>> lcsMap(sizeA + 1, std::vector(sizeB + 1, 0));
        std::vector<std::vector<char>> directions(sizeA + 1, std::vector(sizeB + 1, 'E'));

        for(size_t i = 1; i <= sizeA; i++) {
            for(size_t j = 1; j <= sizeB; j++) {
                if(A[i - 1] == B[j - 1]) {
                    lcsMap[i][j] = lcsMap[i - 1][j - 1] + 1;
                    directions[i][j] = 'D'; // diagonal
                } else {
                    if(lcsMap[i - 1][j] >= lcsMap[i][j - 1]) {
                        lcsMap[i][j] = lcsMap[i - 1][j];
                        directions[i][j] = 'U'; // up
                    } else {
                        lcsMap[i][j] = lcsMap[i][j - 1];
                        directions[i][j] = 'L'; // left
                    }
                }
            }
        }

        return directions;
    }

}
