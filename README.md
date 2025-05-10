# Image-converter

Проект универсального конвертера изображений, который поддерживает конвертацию изображений трёх форматов: BMP, JPEG, PPM.

## Структура проекта

```mermaid
    classDiagram
    namespace formats {
        class ImageFormatInterface {
            <<interface>>
            + SaveImage(const img_lib::Path& file, const img_lib::Image& image) bool*
            + LoadImage(const img_lib::Path& file) img_lib::Image*
        }
        
        class PPMFormatInterface {
            +SaveImage(const img_lib::Path& file, const img_lib::Image& image) bool
            LoadImage(const img_lib::Path& file) img_lib::Image
        }
        
        class JpegFormatInterface {
            +SaveImage(const img_lib::Path& file, const img_lib::Image& image) bool
            +LoadImage(const img_lib::Path& file) img_lib::Image
        }
        
        class BmpFormatInterface {
            +SaveImage(const img_lib::Path& file, const img_lib::Image& image) bool
            +LoadImage(const img_lib::Path& file) img_lib::Image
        }
    }
    namespace img_lib {
        class ImgLib {
            <<static library>>
            +img_lib.h
            +ppm_image.h
            +jpeg_image.h
            +bmp_image.h
            +pack_defines.h
            +...cpp
        }
    }
    class libjpeg {
        <<static library>>
    }
    
    ImageFormatInterface <|.. PPMFormatInterface
    ImageFormatInterface <|.. JpegFormatInterface
    ImageFormatInterface <|.. BmpFormatInterface

    PPMFormatInterface --> ImgLib : dependency
    JpegFormatInterface --> ImgLib : dependency
    BmpFormatInterface --> ImgLib : dependency
    ImgLib --> libjpeg : dependency
```
<br>

### Основные структурные элементы

В проекте реализован интерфейс ImageFormatInterface для чтения и сохранения изображений. Классы PPMFormatInterface, JpegFormatInterface, BmpFormatInterface реализуют виртуальные методы интерфейса для конвертации изображений соответствующего формата. 
В процессе сборки из файлов реализации, связанных с обработкой изображений создается статическая библиотека ImgLib. Для обработки изображений в формате jpeg подключается дополнительная сторонняя библиотека libjpeg, которая представлена в одноименной директории.  

### Используемые инструменты

- OC: Ubuntu 22.04
- cmake: 3.22.1
- компилятор: gcc 11.4.0
- стандарт языка: С++17
- отладчик: GNU gdb 12.1

## Download

Скачать репозиторий можно с помощью команды:

```
git clone git@github.com:alexkozlovvv/cpp-image-converter.git
```

## Usage

При сборке проекта с помощью Cmake необходимо указать конфигурацию сборки параметром -DCMAKE_BUILD_TYPE, а так же путь к директории статической библиотеки libjpeg. При этом предвартельно желательно создать директорию, из которой будут вызываться команды сборки. Для конфигурации debug команды будут выглядеть так:

```
cmake ../ImgConverter -DCMAKE_BUILD_TYPE=Debug -DLIBJPEG_DIR=<путь до директории libjpeg>
cmake --build .
```
По окончании сборки должен появиться исполняемый файл imgconv. 
При запуске программы необходимо передать два параметра. Первый параметр содержит путь до входного изображения. Второй параметр содержит путь до выходного. Выходное изображение будет создано автоматически.

```
./imgconv <path to input image> <path to output image>
```


