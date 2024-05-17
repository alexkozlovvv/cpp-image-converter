#include "ppm_image.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

static const string_view PPM_SIG = "P6"sv;
static const int PPM_MAX = 255;

bool SavePPM(const Path& file, const Image& image) {

    ofstream ofs(file, ios::binary);
    if (!ofs.is_open()) {
        // Обработать ошибку открытия файла
        return false;
    }

    /* Сохраняем заголовок */
    ofs << PPM_SIG << "\n"s << 
           std::to_string(image.GetWidth()) << " "s <<
           std::to_string(image.GetHeight()) << "\n"s <<
           std::to_string(PPM_MAX) << "\n"s;
    
    /* Сохраняем значения пикселей изображения */
    std::vector<char> buff(image.GetWidth() * 3);

    for (int y = 0; y < image.GetHeight(); ++y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < image.GetWidth(); ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].r);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].b);
        }
        /* Переадем буфер в файл */
        ofs.write(buff.data(), image.GetWidth() * 3);
    }
    return ofs.good();
}

Image LoadPPM(const Path& file) {

    std::string sign;
    int w, h, color_max;

    ifstream ifs(file, ios::binary);
    if (!ifs.is_open()) {
        // Обработать ошибку открытия файла
        return {};
    }

    // читаем заголовок: он содержит формат, размеры изображения
    // и максимальное значение цвета
    ifs >> sign >> w >> h >> color_max;

    // мы поддерживаем изображения только формата P6
    // с максимальным значением цвета 255
    if (sign != PPM_SIG || color_max != PPM_MAX) {
        return {};
    }

    // пропускаем один байт - это конец строки
    const char next = ifs.get();
    if (next != '\n') {
        return {};
    }

    Image result(w, h, Color::Black());
    std::vector<char> buff(w * 3);

    for (int y = 0; y < h; ++y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), w * 3);
        for (int x = 0; x < w; ++x) {
            line[x].r = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].b = static_cast<byte>(buff[x * 3 + 2]);
        }
    }
    if(!ifs.good()) {
        // Обработать ошибку чтения из файла
        return {};
    }
    return result;
}

}  // namespace img_lib
