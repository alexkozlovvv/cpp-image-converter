#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BitmapFileHeader {
    char sign[2];
    unsigned int header_and_data_size;
    int reserved;
    unsigned int indent;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    unsigned int second_part_header_size;
    int image_width;
    int image_height;
    unsigned short plane_number;
    unsigned short bits_per_pixel;
    unsigned int compression_type;
    unsigned int data_byte_count;
    int h_resolution;
    int v_resolution;
    int number_of_color;
    int numder_of_significant_color;
}
PACKED_STRUCT_END

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

bool SaveBMP(const Path& file, const Image& image) {
    BitmapFileHeader file_feader;
    BitmapInfoHeader info_header;

    /* Открываем файл для записи */
    ofstream ofs(file, ios::binary);
    if (!ofs.is_open()) {
        // Обработать ошибку открытия файла
        return false;
    }

    const int im_width = image.GetWidth();
    const int im_height = image.GetHeight();
    const auto stride = GetBMPStride(image.GetWidth());
    const auto file_header_size = sizeof(BitmapFileHeader);
    const auto info_header_size = sizeof(BitmapInfoHeader);

    /* Заполняем заголовки */
    file_feader.sign[0] = 'B';
    file_feader.sign[1] = 'M';
    file_feader.header_and_data_size = file_header_size + info_header_size + 
                                       (stride * im_height);
    file_feader.reserved = 0;
    file_feader.indent = file_header_size + info_header_size;

    info_header.second_part_header_size = info_header_size;
    info_header.image_width = im_width;
    info_header.image_height = im_height;
    info_header.plane_number = 1;
    info_header.bits_per_pixel = 24;
    info_header.compression_type = 0;
    info_header.data_byte_count = stride * im_height;
    info_header.h_resolution = 11811;
    info_header.v_resolution = 11811;
    info_header.number_of_color = 0;
    info_header.numder_of_significant_color = 0x1000000;
    
    /* Записываем заголовки */
    ofs.write(reinterpret_cast<char*>(&file_feader), sizeof(file_feader));
    ofs.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    
    /* Записываем данные */
    std::vector<char> buff(stride);

    for (int y = im_height - 1; y >= 0; y--) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < im_width; x++) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        ofs.write(buff.data(), stride);
    }
    return ofs.good();
}

Image LoadBMP(const Path& file) {
    BitmapFileHeader file_feader;
    BitmapInfoHeader info_header;
    
    /* Открываем файл */
    ifstream ifs(file, ios::binary);

    if (!ifs.is_open()) {
        // Обработать ошибку открытия файла
        return {};
    }

    /* Читаем заголовки */
    ifs.read(reinterpret_cast<char*>(&file_feader), sizeof(file_feader));
    ifs.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    const auto stride = GetBMPStride(info_header.image_width);

    /* Читаем данные */
    Image result(info_header.image_width, info_header.image_height, Color::Black());
    std::vector<char> buff(stride);

    for (int y = info_header.image_height - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), stride);
        for (int x = 0; x < info_header.image_width; ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }
    if(!ifs.good()) {
        // Обработать ошибку чтения из файла
        return {};
    }
    return result;
}

}  // namespace img_lib