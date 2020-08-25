#include "DiffObj.h"
#include "BinObj.h"
#include "ImageObj.h"
#include "FolderObj.h"
#include "TextObj.h"
#include <stdexcept>

namespace diff {

    std::unique_ptr<DiffObj> DiffObj::createByMimeType(const std::string & path, const ArgParser & ap, int recursionLimit) {
        std::filesystem::path fPath(path);
        std::string extension = fPath.extension();
        std::unique_ptr<DiffObj> tmp;
        std::string forceFlag;

        if(std::filesystem::is_regular_file(fPath)) {
            if(ap.getFlag(constants::FLAG_FORCE,forceFlag)) {
                if(forceFlag == "bin") {
                    tmp = std::make_unique<BinObj>(fPath);
                } else {
                    throw std::runtime_error(constants::RTEX_INVALID_FLAG_OPTION);
                }
            } else {
                if(extension == ".bmp") {
                    tmp = std::make_unique<ImageObj>(fPath);
                } else if(extension == ".txt") {
                    tmp = std::make_unique<TextObj>(fPath, ap);
                } else {
                    tmp = std::make_unique<BinObj>(fPath);
                }
            }
        } else if(std::filesystem::is_directory(fPath)) {
            tmp = std::make_unique<FolderObj>(path, ap, recursionLimit);
        } else {
            throw std::runtime_error(constants::RTEX_INVALID_INPUT_FILE);
        }

        return tmp;
    }

    bool DiffObj::isHidden(const std::filesystem::path & path) {
        std::string fname(path.filename());
        if(fname.empty()) {
            return false;
        }
        return fname[0] == '.';
    }

}
