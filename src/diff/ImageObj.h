#ifndef HEADER_DIFF_IMAGEOBJ
#define HEADER_DIFF_IMAGEOBJ
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <filesystem>
#include "DiffObj.h"
// working with images heavily inspired by https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/

namespace diff {

    class ImageObj : public DiffObj  {

        private:
            std::vector<uint8_t> pixels;

            #pragma pack(push, 1)
            struct BMPFileHeader {
                uint16_t fileType{0x4D42};
                uint32_t fileSize{0};
                uint16_t reserved1{0};
                uint16_t reserved2{0};
                uint32_t pixelOffset{0};
            } fileHeader;

            struct BMPInfoHeader {
                uint32_t infoHeaderSize{0};
                int32_t bitmapWidth{0};
                int32_t bitmapHeight{0};
                uint16_t planesCount{1};
                uint16_t bitPerPixelCount{0};
                uint32_t compression{0};
                uint32_t imageSize{0};
                int32_t xPixelsPerMeter{0};
                int32_t yPixelsPerMeter{0};
                uint32_t usedColors{0};
                uint32_t importantColors{0};
            } infoHeader;

            struct BMPColorHeader {
                uint32_t redMask{0x00ff0000};
                uint32_t greenMask{0x0000ff00};
                uint32_t blueMask{0x000000ff};
                uint32_t alphaMask{0xff000000};
                uint32_t colorSpaceType{0x73524742};
                uint32_t unusedColors[16]{0};
            } colorHeader;
            #pragma pack(pop)

            //! Byte rowWidth for saving into files  
            uint32_t rowWidth{0};
            
            /**
             * @brief Loads header data into class instance its called on
             * 
             * @param is 
             */
            void readHeaders(std::ifstream & is);
            
            /**
             * @brief Verifies that the color header is supported
             * 
             */
            void verifyColorHeader() const;

            /**
             * @brief Loads pixel data from ifstream to the pixel vector
             * 
             * @param is input file stream
             */
            void readPixels(std::ifstream & is);

            /**
             * @brief Writes image header to output stream
             * 
             * @param os output stream
             */
            void writeHeaders(std::ostream & os) const;
            
            /**
             * @brief aligns rowWidth to be divisible by provided value
             * 
             * @param alignTo divisor
             * @return uint32_t aligned width value
             */
            uint32_t alignWidth(uint32_t alignTo) const;

            /**
             * @brief Processes two images into one with given mode
             * 
             * @param res processed image
             * @param lhs left image to process
             * @param rhs right image to process
             * @param mode processing mode (avg | sub)
             */
            void processImages(std::unique_ptr<ImageObj> & res, const ImageObj & lhs, const ImageObj & rhs, std::string & mode) const;
            
        public:

            /**
             * @brief Construct a new ImageObj object, creates stream and loads image structure
             * 
             * @param path 
             */
            ImageObj(const std::filesystem::path & path);
            
            /**
             * @brief Construct a new blank ImageObj object with same headers as image provided
             * 
             * @param img image to take headers from
             */
            ImageObj(const ImageObj & img);
            
            std::unique_ptr<DiffObj> diff(DiffObj & rhs, const ArgParser & ap) const override;

            void print(std::ostream & os, const ArgParser & ap) const override;

    };

}

#endif
