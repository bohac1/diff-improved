#ifndef HEADER_DIFF_CONSTANTS
#define HEADER_DIFF_CONSTANTS
#include <cstring>
#include <string>
#include <map>

/**
 * @brief namespace containing constants definitions
 * 
 */
namespace diff::constants {

    const std::string FLAG_OUT = "--out";
    const std::string FLAG_FORMAT = "--format";
    const std::string FLAG_FORCE = "--force";
    const std::string FLAG_DEPTH = "--depth";
    const std::string FLAG_IMAGE_MODE = "--image-mode";
    const std::string FLAG_IGNORE_HIDDEN = "--ignore-hidden";
    const std::string FLAG_STRUCTURE_ONLY = "--structure-only";
    const std::string FLAG_IGNORE_EMPTY_LINE = "--ignore-empty-lines";

    const std::string TEXTOBJ_OPERATION_ADD = "+ ";
    const std::string TEXTOBJ_OPERATION_REM = "- ";
    const std::string FOLDEROBJ_OPERATION_ADD = "+ ";
    const std::string FOLDEROBJ_OPERATION_REM = "- ";
    const std::string OBJECTS_NOT_OF_THE_SAME_TYPE = " are not of the same type";
    const std::string OBJECTS_NAME_FILE_PRETEXT = "start diffing object: ";
    const std::string OBJECTS_NAME_FILE_POSTEXT = "end diffing object: ";
    const std::string OBJECTS_NAME_FOLDER_PRETEXT = "cd ";
    const std::string OBJECTS_NAME_FOLDER_POSTEXT = "cd ..";

    //! ArgParser map of flags for input checking <name of flag, value required> 
    const std::map<std::string, bool> FLAG_DEF = {
        {std::string(FLAG_OUT), true},
        {std::string(FLAG_FORMAT), true},
        {std::string(FLAG_FORCE), true},
        {std::string(FLAG_DEPTH), true},
        {std::string(FLAG_IMAGE_MODE), true},
        {std::string(FLAG_IGNORE_HIDDEN), false},
        {std::string(FLAG_STRUCTURE_ONLY), false},
        {std::string(FLAG_IGNORE_EMPTY_LINE), false}
    };

    //! invalid arguments runtime exception messages
    const std::string RTEX_INVALID_ARG_COUNT = "invalid number of arguments";
    const std::string RTEX_INVALID_FLAG = "invalid flag provided";
    const std::string RTEX_INVALID_FLAG_OPTION = "invalid flag option provided";
    const std::string RTEX_DIFFERENT_PIXEL_DEPTH = "both images has to use the same color depth";
    const std::string RTEX_INVALID_INPUT_FILE = "path provided doesn't point to file or directory";

    //! other runtime exception messages
    const std::string RTEX_FSTREAM_OPEN_ERR = "could not open the file";
    const std::string RTEX_FSTREAM_READ_ERR = "could not read from the stream";
    const std::string RTEX_IFSTREAM_OPEN_ERR = "could not write to the file";
    const std::string RTEX_IFSTREAM_WRITE_ERR = "could not write to the stream";
    const std::string RTEX_NOT_BMP_FILE = "Provided file type is not of BMP";
    const std::string RTEX_UNSUPPORTED_IMG_COMPRESSION = "Images compression is not supported";
    const std::string RTEX_IMG_MISSING_COLOR_HEADER = "missing color header";
    const std::string RTEX_IMG_UNEXPECTED_COLOR_MASK_FORMAT = "Unexpected color mask format.";
    const std::string RTEX_IMG_UNEXPECTED_COLOR_SPACE_TYPE = "Unexpected color space type.";
    const std::string RTEX_IMG_UNSUPPORTED_ORIGIN = "Unsupported image origin.";
    const std::string RTEX_IMG_UNSUPPORTED_COLOR_DEPTH = "Not a 24 or 32 bits per pixel BMP file";
    const std::string RTEX_DIFF_FILES_ARE_NOT_OF_THE_SAME_TYPE = "Input files are not of the same type";

}

#endif
