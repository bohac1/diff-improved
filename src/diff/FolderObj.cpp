#include "FolderObj.h"
#include "TextObj.h"
#include <typeinfo>
#include <filesystem>

namespace diff {

    FolderObj::FolderObj(const std::filesystem::path & path, const ArgParser & ap, int recursionLimit) {
        fileName = path.filename();
        std::string tmp;

        for(const auto& file : std::filesystem::directory_iterator(path)) {
            // continue the loop if the current file is hidden and flag to skip hidden files is present
            if((ap.getFlag(constants::FLAG_IGNORE_HIDDEN, tmp) && DiffObj::isHidden(file.path()))
                || (ap.getFlag(constants::FLAG_STRUCTURE_ONLY, tmp) && !std::filesystem::is_directory(file.path()))) {
                continue;
            }
            if(recursionLimit != 0) {
                structure.insert(std::pair<std::string, std::unique_ptr<DiffObj>>(file.path().filename(), DiffObj::createByMimeType(file.path(), ap, recursionLimit - 1)));
            }
        }
    }

    std::unique_ptr<DiffObj> FolderObj::diff(DiffObj & rhs, const ArgParser & ap) const {
        auto tmp = std::make_unique<TextObj>();
        std::stringstream ss;
        FolderObj & frhs = dynamic_cast<FolderObj &>(rhs);

        for(const auto & it : structure) {
            auto secondIt = frhs.structure.find(it.first);
            if(secondIt != frhs.structure.end()) {
                if(typeid(* it.second).hash_code() == typeid(* secondIt->second).hash_code()) {
                    // objects are of the same type
                    if(typeid(FolderObj).hash_code() == typeid(* it.second).hash_code()) {
                        // is folder object
                        ss << constants::OBJECTS_NAME_FOLDER_PRETEXT << it.first << std::endl;
                        it.second->diff(*(secondIt->second), ap)->print(ss, ap);
                        ss << constants::OBJECTS_NAME_FOLDER_POSTEXT << std::endl;
                    } else {
                        // is other than folder object
                        ss << constants::OBJECTS_NAME_FILE_PRETEXT << it.first << std::endl;
                        it.second->diff(*(secondIt->second), ap)->print(ss, ap);
                        ss << constants::OBJECTS_NAME_FILE_POSTEXT << it.first << std::endl;
                    }
                } else {
                    ss << it.first << constants::OBJECTS_NOT_OF_THE_SAME_TYPE << std::endl;
                }
            } else {
                // not in frhs structure
                ss << constants::FOLDEROBJ_OPERATION_ADD + it.first << std::endl;
            }
        }

        for(const auto & it : frhs.structure) {
            auto secondIt = structure.find(it.first);
            if(secondIt == structure.end()) {
                // not in this structure
                ss << constants::FOLDEROBJ_OPERATION_REM + it.first << std::endl;
            }
        }
        tmp->loadFromIstream(ss);

        return tmp;
    }

    void FolderObj::print(std::ostream & , const ArgParser & ) const {
    }

}
