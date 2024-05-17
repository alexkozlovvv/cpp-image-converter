#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

namespace formats {

    class ImageFormatInterface {
    public:
        virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
        virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
    };

    class PPMFormatInterface : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {
            return img_lib::SavePPM(file, image);
        }
        img_lib::Image LoadImage(const img_lib::Path& file) const {
            return img_lib::LoadPPM(file);
        }
    };

    class JpegFormatInterface : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {
            return img_lib::SaveJPEG(file, image);
        }
        img_lib::Image LoadImage(const img_lib::Path& file) const {
            return img_lib::LoadJPEG(file);
        }
    };

    class BmpFormatInterface : public ImageFormatInterface {
    public:
        bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const {
            return img_lib::SaveBMP(file, image);
        }
        img_lib::Image LoadImage(const img_lib::Path& file) const {
            return img_lib::LoadBMP(file);
        }
    };
}

enum class Format{
    PPM,
    JPEG,
    BMP,
    UNKNOWN
};


static const Format ppmInterface = Format::PPM;
static const Format jpegInterface = Format::JPEG;
static const Format bmpInterface = Format::BMP;
  

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    if (ext == ".bmp"sv) {
        return Format::BMP;
    }

    return Format::UNKNOWN;
}

formats::ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {

    auto fileformat = GetFormatByExtension(path);
    switch(fileformat)
    {
        case ppmInterface:
            return new formats::PPMFormatInterface();
        case jpegInterface:
            return new formats::JpegFormatInterface();
        case bmpInterface:
            return new formats::BmpFormatInterface();
    }
    return nullptr;
}

int main(int argc, const char** argv) {

    formats::ImageFormatInterface* in_image_format_ptr;
    formats::ImageFormatInterface* out_image_format_ptr;

    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    in_image_format_ptr = GetFormatInterface(in_path);

    if(in_image_format_ptr == nullptr) {
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }

    out_image_format_ptr = GetFormatInterface(out_path);

    if(out_image_format_ptr == nullptr) {
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }

    img_lib::Image image = in_image_format_ptr->LoadImage(in_path);
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }

    if (!out_image_format_ptr->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}
