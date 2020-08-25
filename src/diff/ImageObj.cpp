#include "ImageObj.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace diff {

    ImageObj::ImageObj(const std::filesystem::path & path) {
        fileName = path.filename();
        std::ifstream is(path, std::ifstream::binary);
        if(is) {
            readHeaders(is);
            readPixels(is);
            if(!is.good()) {
                throw std::runtime_error(constants::RTEX_FSTREAM_READ_ERR);
            }
        } else {
            throw std::runtime_error(constants::RTEX_FSTREAM_OPEN_ERR);
        }
    }

    ImageObj::ImageObj(const ImageObj & img) {
        fileHeader = img.fileHeader;
        infoHeader = img.infoHeader;
        colorHeader = img.colorHeader;
        rowWidth = img.rowWidth;
        pixels.reserve(img.pixels.size());
    }

    void ImageObj::readHeaders(std::ifstream & is) {
        is.read((char *) &fileHeader, sizeof(fileHeader));
        if(fileHeader.fileType != 0x4D42) {
            throw std::runtime_error(constants::RTEX_NOT_BMP_FILE);
        }
        is.read((char *) &infoHeader, sizeof(infoHeader));
        if(infoHeader.compression != 0 && infoHeader.compression != 3) {
            throw std::runtime_error(constants::RTEX_UNSUPPORTED_IMG_COMPRESSION);
        }
        // check if loaded header contains color header (color header in 32 bit color images)
        if(infoHeader.bitPerPixelCount == 32) {
            if(infoHeader.infoHeaderSize >= (sizeof(BMPColorHeader) + sizeof(BMPInfoHeader))) {
                is.read((char *) &colorHeader, sizeof(colorHeader));
                verifyColorHeader();
            } else {
                throw std::runtime_error(constants::RTEX_IMG_MISSING_COLOR_HEADER);
            }
        }
    }

    void ImageObj::verifyColorHeader() const {
        BMPColorHeader expectation;
        if(!(expectation.alphaMask == colorHeader.alphaMask || colorHeader.alphaMask == 0x0) 
        || expectation.blueMask  != colorHeader.blueMask
        || expectation.redMask   != colorHeader.redMask
        || expectation.greenMask != colorHeader.greenMask) {
            throw std::runtime_error(constants::RTEX_IMG_UNEXPECTED_COLOR_MASK_FORMAT);
        } else if(expectation.colorSpaceType != colorHeader.colorSpaceType) {
            throw std::runtime_error(constants::RTEX_IMG_UNEXPECTED_COLOR_SPACE_TYPE);
        }
    }

    void ImageObj::readPixels(std::ifstream & is) {
        is.seekg(fileHeader.pixelOffset, is.beg);
        if(infoHeader.bitmapHeight < 0 || infoHeader.bitmapWidth < 0) {
            throw std::runtime_error(constants::RTEX_IMG_UNSUPPORTED_ORIGIN);
        }
        // resize pixel vector to fit the pixels
        pixels.resize((infoHeader.bitmapWidth * infoHeader.bitmapHeight * infoHeader.bitPerPixelCount) / 8);

        // get rid of junk that some image editors save to headers
        if(infoHeader.bitPerPixelCount == 32) {
            infoHeader.infoHeaderSize = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
            fileHeader.pixelOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        } else {
            infoHeader.infoHeaderSize = sizeof(BMPInfoHeader);
            fileHeader.pixelOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
        }
        fileHeader.fileSize = fileHeader.pixelOffset;

        // read only pixels to vector
        if(infoHeader.bitmapWidth % 4 == 0) {
            is.read((char *) pixels.data(), pixels.size());
            fileHeader.fileSize += pixels.size();
        } else {
            rowWidth = infoHeader.bitmapWidth * infoHeader.bitPerPixelCount / 8;
            // calculate padding bytes
            uint32_t newWidth = alignWidth(4);
            std::vector<uint8_t> paddingRow(newWidth - rowWidth);
            // read to pixels and get rid of padded data
            for(int y = 0; y < infoHeader.bitmapHeight; ++y) {
                is.read((char *) (pixels.data() + rowWidth * y), rowWidth);
                is.read((char *) paddingRow.data(), paddingRow.size());
            }
            fileHeader.fileSize += pixels.size() + infoHeader.bitmapHeight * paddingRow.size();
        }
    }

    uint32_t ImageObj::alignWidth(uint32_t alignTo) const{
        uint32_t newWidth = rowWidth;
        // add 1 untill aligned
        while(newWidth % alignTo != 0) {
            newWidth++;
        }
        return newWidth;
    }
 
    void ImageObj::writeHeaders(std::ostream & os) const {
        os.write((const char*)&fileHeader, sizeof(fileHeader));
        os.write((const char*)&infoHeader, sizeof(infoHeader));
        if(infoHeader.bitPerPixelCount == 32) {
            os.write((const char*)&colorHeader, sizeof(colorHeader));
        }
    }

    std::unique_ptr<DiffObj> ImageObj::diff(DiffObj & rhs, const ArgParser & ap) const {
        ImageObj & irhs = dynamic_cast<ImageObj &>(rhs);
        if(infoHeader.bitPerPixelCount!=irhs.infoHeader.bitPerPixelCount) {
            throw std::runtime_error(constants::RTEX_DIFFERENT_PIXEL_DEPTH);
        }
        // make copy of current instance
        auto ptr = std::make_unique<ImageObj>(* this);
        // diff objects
        std::string imageMode;
        if(!ap.getFlag("--image-mode", imageMode)) {
            // set substraction as default image diff mode
            imageMode = "sub";
        }
        processImages(ptr, * this, irhs, imageMode);
        return ptr;
    }

    void ImageObj::processImages(std::unique_ptr<ImageObj> & res, const ImageObj & lhs, const ImageObj & rhs, std::string & mode) const {
        uint64_t currentLhsPosition;
        uint64_t currentRhsPosition;
        uint64_t minBitmapWidthPx = (std::min(lhs.infoHeader.bitmapWidth, rhs.infoHeader.bitmapWidth) * lhs.infoHeader.bitPerPixelCount) / 8;
        int32_t minBitmapHeight = std::min(lhs.infoHeader.bitmapHeight, rhs.infoHeader.bitmapHeight);
        uint64_t resBmapWidthPx = (lhs.infoHeader.bitmapWidth * lhs.infoHeader.bitPerPixelCount) / 8;
        uint64_t rhsBmapWidhtPx = (rhs.infoHeader.bitmapWidth * rhs.infoHeader.bitPerPixelCount) / 8;
        // iteration vars
        uint64_t refWidth;
        int32_t refHeight;
        // substract and average splited into 2 almost the same blocks for performance reasons
        if(mode == "sub") {
            for(refHeight = 0; refHeight < minBitmapHeight; ++refHeight) {
                for(refWidth = 0; refWidth < minBitmapWidthPx; ++refWidth) {
                    currentLhsPosition = refWidth + refHeight * resBmapWidthPx;
                    currentRhsPosition = refWidth + refHeight * rhsBmapWidhtPx;
                    res->pixels.push_back(
                        lhs.pixels[currentLhsPosition] > rhs.pixels[currentRhsPosition]
                            ? lhs.pixels[currentLhsPosition] - rhs.pixels[currentRhsPosition]
                            : 0
                        );
                }
                for(; refWidth < resBmapWidthPx; ++refWidth) {
                    res->pixels.push_back(lhs.pixels[refWidth + refHeight * resBmapWidthPx]);
                }
            } 
        } else if(mode == "avg") {
            for(refHeight = 0; refHeight < minBitmapHeight; ++refHeight) {
                for(refWidth = 0; refWidth < minBitmapWidthPx; ++refWidth) {
                    currentLhsPosition = refWidth + refHeight * resBmapWidthPx;
                    currentRhsPosition = refWidth + refHeight * rhsBmapWidhtPx;
                    res->pixels.push_back((lhs.pixels[currentLhsPosition] + rhs.pixels[currentRhsPosition]) / 2);
                }
                for(; refWidth < resBmapWidthPx; ++refWidth) {
                    res->pixels.push_back(lhs.pixels[refWidth + refHeight * resBmapWidthPx]);
                }
            } 
        } else {
            throw std::runtime_error(constants::RTEX_INVALID_FLAG_OPTION);
        }

        // fill remaining pixels with lhs picture
        for(; refHeight < lhs.infoHeader.bitmapHeight; ++refHeight) {
            for(refWidth = 0; refWidth < resBmapWidthPx; ++refWidth) {
                res->pixels.push_back(lhs.pixels[refWidth + refHeight * resBmapWidthPx]);
            }
        }
    }

    void ImageObj::print(std::ostream & os, const ArgParser & ) const {
        if(infoHeader.bitPerPixelCount == 32) {
            writeHeaders(os);
            os.write((const char *)pixels.data(), pixels.size());
        } else if(infoHeader.bitPerPixelCount == 24) {
            if(infoHeader.bitmapWidth % 4 == 0) {
                writeHeaders(os);
                os.write((const char *)pixels.data(), pixels.size());
            } else {
                uint32_t newWidth = alignWidth(4);
                std::vector<uint8_t> padding_row(newWidth - rowWidth);
                writeHeaders(os);

                for(int y = 0; y < infoHeader.bitmapHeight; ++y) {
                    os.write((const char *) (pixels.data() + rowWidth * y), rowWidth);
                    os.write((const char *) padding_row.data(), padding_row.size());
                }
            }
        } else {
            throw std::runtime_error(constants::RTEX_IMG_UNSUPPORTED_COLOR_DEPTH);
        }
    }

}
