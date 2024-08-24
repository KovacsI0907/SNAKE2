#pragma once
#include <vectors.h>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

class ImageNotLoadedException : public std::exception {
public:
	const char* what() const override {
		return "Image has not been loaded yet";
	}
};

class BitMapLoaderException : public std::exception {
private:
	std::string message;

public:
	explicit BitMapLoaderException(const std::string& msg) : message(msg) {}

	const char* what() const override {
		return message.c_str();
	}
};


class Image {
public:
	virtual void load() = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;
	virtual vec4 getPixel(unsigned int x, unsigned int y) const = 0;
	virtual const float* getPixelArray() const = 0;
	virtual ~Image() {};
};

class BmpImage : public Image{
	std::vector<vec4> data;
	unsigned int width = 0;
	unsigned int height = 0;
	std::filesystem::path path;

	unsigned int readInt(const char* buf, unsigned int offset) const {
		return ((int)buf[offset] & 0xFF) |
			((int)buf[offset + 1] & 0xFF) << 8 |
			((int)buf[offset + 2] & 0xFF) << 16 |
			((int)buf[offset + 3] & 0xFF) << 24;
	}

	vec4 read24bitBGR(const unsigned char* pixelBytes, unsigned int blueOffset) const {
		return vec4((pixelBytes[blueOffset + 2])/256.0f, pixelBytes[blueOffset + 1]/256.0f, pixelBytes[blueOffset]/256.0f, 1.0f);
	}

public:
	explicit BmpImage(const std::filesystem::path pathToImage) : path(pathToImage) {};

	void load() override {
		std::ifstream file(path, std::ios::binary);

		// TODO should file exceptions be handled?

		if (!file.is_open()) {
			throw BitMapLoaderException("File '" + path.string() + "' can't be opened");
		}

		char headerBuf[54];

		file.read(headerBuf, 54);

		if (!file.good()) {
			throw BitMapLoaderException("Error reading header of file '" + path.string() + "'");
		}
		
		// check if the header starts with the bitmap magic value "BM"
		if (headerBuf[0] != 'B' || headerBuf[1] != 'M') {
			throw BitMapLoaderException("Error in bitmap header");
		}

		// check if DIB header size == 40 (meaning it is the Windows BITMAPINFOHEADER)
		if (readInt(headerBuf, 0x0E) != 40) {
			throw BitMapLoaderException("Unsupported DIB header type");
		}
		
		// check if compression method == 0 (meaning no compression)
		if (readInt(headerBuf, 0x1E) != 0) {
			throw BitMapLoaderException("Compressed bitmaps are not supported");
		}

		// check if number of colors in the palette == 0 (meaning no color palette)
		if (readInt(headerBuf, 0x2E) != 0) {
			throw BitMapLoaderException("Color palette based bitmaps are not supported");
		}

		//check if bits per pixel is 24 (24 bps && no compression => BGR pixel data layout)
		if (headerBuf[0x1C] != 24 || headerBuf[0x1D] != 0) {
			throw BitMapLoaderException("Only 24 bit color is supported");
		}
		unsigned int bitsPerPixel = 24;

		//at this point we know the image is in the supported format
		unsigned int pixelDataStart = readInt(headerBuf, 0x0A);
		unsigned int bytesToDiscard = pixelDataStart - 54;
		width = readInt(headerBuf, 0x12);
		height = readInt(headerBuf, 0x16);

		//skip unnecessary headers
		file.ignore(bytesToDiscard);

		// row data is always padded so that the num of bytes is divisible by 4
		int rowSizeBytes = ((bitsPerPixel * width + 31) / 32) * 4;

		char* pixelBuffer = new char[rowSizeBytes * height];
		unsigned char const* unsignedPixelBuf = (unsigned char*)pixelBuffer;
		file.read(pixelBuffer, rowSizeBytes * height);
		data.reserve(width * height);

		for (int rowIndex = 0; rowIndex < height; rowIndex++) {
			int rowStart = rowIndex * rowSizeBytes;
			for (int x = 0; x < width; x++) {
				data.push_back(read24bitBGR(unsignedPixelBuf, rowStart + x * 3));
			}
		}

		delete[] pixelBuffer;
	}

	~BmpImage() override = default;

	void assertLoaded() const {
		if (data.empty()) {
			throw ImageNotLoadedException();
		}
	}

	unsigned int getWidth() const override {
		assertLoaded();
		return width;
	}
	unsigned int getHeight() const override {
		assertLoaded();
		return height;
	}

	vec4 getPixel(const unsigned int x, const unsigned int y) const override {
		assertLoaded();
		return data[y * width + x];
	}

	const float* getPixelArray() const override {
		assertLoaded();
		return &data[0].x;
	}
};