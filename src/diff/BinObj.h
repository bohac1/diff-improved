#ifndef HEADER_DIFF_BINOBJ
#define HEADER_DIFF_BINOBJ
#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <memory>
#include "DiffObj.h"


namespace diff {

    class BinObj : public DiffObj  {

        private:
            //! bytes of binnary file (object)
            std::vector<uint8_t> bytes;

        public:

            /**
             * @brief Construct a new BinObj object, fills bytes vector from file in provided path
             * 
             * @param path to file
             */
            BinObj(const std::filesystem::path & path);

            /**
             * @brief Construct a new BinObj object, reserves size for bytes vector
             * 
             * @param size size to reserve
             */
            BinObj(const size_t size);

            std::unique_ptr<DiffObj> diff(DiffObj & rhs, const ArgParser & ap) const override;

            void print(std::ostream & os, const ArgParser & ap) const override;
    };

}

#endif
