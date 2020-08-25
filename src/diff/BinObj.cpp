#include "BinObj.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <iomanip>


namespace diff {

    BinObj::BinObj(const std::filesystem::path & path) {
        fileName = path.filename();
        std::ifstream is(path, std::ifstream::binary);
        if(is) {
            // get length of file:
            std::filesystem::file_size(path);
            int len = std::filesystem::file_size(path);
            // read whole file to char vector
            bytes.resize(len);
            is.read((char *) bytes.data(), bytes.size());
            if(!is.good()) {
                throw std::runtime_error(constants::RTEX_FSTREAM_READ_ERR);
            }
        } else {
            throw std::runtime_error(constants::RTEX_FSTREAM_OPEN_ERR);
        }
    }

    BinObj::BinObj(const size_t size) {
        bytes.reserve(size);
    }

    std::unique_ptr<DiffObj> BinObj::diff(DiffObj & rhs, const ArgParser & ) const {
        BinObj & brhs = dynamic_cast<BinObj &>(rhs);
        bool currBigger = bytes.size() > brhs.bytes.size(); 
        size_t fSize = currBigger ? bytes.size() : brhs.bytes.size();
        auto ptr = std::make_unique<BinObj>(fSize);
        size_t minSize = currBigger ? brhs.bytes.size() : bytes.size();
        size_t sizeIt;
        for(sizeIt = 0; sizeIt < minSize; ++sizeIt) {
            ptr->bytes.push_back(
                    bytes[sizeIt] - brhs.bytes[sizeIt]
                );
        }
        if(currBigger) {
            ptr->bytes.insert(ptr->bytes.begin() + sizeIt, bytes.begin() + sizeIt, bytes.end()); 
        } else {
            size_t maxSize = brhs.bytes.size();
            for(; sizeIt < maxSize; ++sizeIt) {
                ptr->bytes.push_back(
                    0 - brhs.bytes[sizeIt]
                );
            }
        }
        return ptr;
    }

    void BinObj::print(std::ostream & os, const ArgParser & ap) const {
        std::string outFormat;
        if(ap.getFlag(constants::FLAG_FORMAT, outFormat)) {
            os << std::setfill('0');
            uint setWidth;
            if(outFormat == "hex") {
                os << std::hex;
                setWidth = 2;
            } else if(outFormat == "dec") {
                os << std::dec;
                setWidth = 3;
            } else if(outFormat == "oct") {
                os << std::oct;
                setWidth = 3;
            } else {
                throw std::runtime_error(constants::RTEX_INVALID_FLAG_OPTION);
            }
            uint32_t byteCount = 0;
            for(auto &byte : bytes) {
                if(byteCount % 8 == 0) {
                    os << std::endl;
                } else if(byteCount % 2 == 0) {
                    os << ' ';
                }
                os << std::setw(setWidth) << (int) byte;
                byteCount++;
            }

        } else {
            os.write((const char*)bytes.data(), bytes.size());
        }
    }

}
