#ifndef HEADER_DIFF_FOLDEROBJ
#define HEADER_DIFF_FOLDEROBJ
#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <filesystem>
#include "DiffObj.h"


namespace diff {

    class FolderObj : public DiffObj  {

        private:

            //! folder structure of DiffObj objects
            std::map<std::string, std::unique_ptr<DiffObj>> structure;

        public:

            /**
             * @brief Construct a new FolderObj object
             * 
             * @param path path to base folder
             * @param ap arguments modifying object creation such as --force=bin
             * @param recursionLimit depth limit for recursion
             */
            FolderObj(const std::filesystem::path & path, const ArgParser & ap, int recursionLimit);

            std::unique_ptr<DiffObj> diff(DiffObj & rhs, const ArgParser & ap) const override;

            void print(std::ostream & os, const ArgParser & ap) const override;
    };

}

#endif
