#ifndef HEADER_DIFF_DIFFOBJ
#define HEADER_DIFF_DIFFOBJ
#include <string>
#include <iostream>
#include <memory>
#include <filesystem>
#include "ArgParser.h"
#include "constants/constants.h"

namespace diff {

    class DiffObj {
        public:
            //! name of the file
            std::string fileName;

            /**
             * @brief Destroy the DiffObj object
             * 
             */
            virtual ~DiffObj() = default;

            /**
             * @brief Differs rhs DiffObj from the current instance based on ArgParser arguments
             * 
             * @param rhs 
             * @param ap 
             * @return std::unique_ptr<DiffObj> created new instance of DiffObj that holds the difference of DiffObj objects rhs and this.
             * @throw std::bad_cast if DiffObj objects are not of the same instance
             */
            virtual std::unique_ptr<DiffObj> diff(DiffObj & rhs, const ArgParser & ap) const = 0;

            /**
             * @brief Creates pointer to DiffObj object based on arguments passed to program and files extensions
             * 
             * @param path path to file
             * @param ap argparser options can be used for forcing different conversions
             * @param recursionLimit limit of recursive lookup for FolderObj creation (-1) for no limit
             * @return std::unique_ptr<DiffObj> 
             */
            static std::unique_ptr<DiffObj> createByMimeType(const std::string & path, const ArgParser & ap, int recursionLimit = -1);

            /**
             * @brief writes DiffObj instance to the ostream output in objects format based on argumets passed
             * 
             * @param os 
             * @param ap 
             */
            virtual void print(std::ostream & os, const ArgParser & ap) const = 0;

            /**
             * @brief Checks if the path is pointing to a hidden file
             * 
             * @param path path to file
             * @return true file is hidden
             * @return false file is not hidden
             */
            static bool isHidden(const std::filesystem::path & path);

    };

}

#endif
