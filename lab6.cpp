#include "lab6.h"

namespace Apa {

    unsigned char *append_bits(char *result, char *bits, int bits_length, int last_bit, int last_byte) {
        return nullptr;
    }

    void writeBMP(const char* res) {


        unsigned char header[62] =   {0x42, 0x4d, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
                                      0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
                                      0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0xff, 0xff, 0xff, 0x00};
        unsigned char search_template[8] = {
                 0b10000000, 0b10111110, 0b10100010, 0b10100010, 0b10100010, 0b10111110, 0b10000000, 0b11111111
        };
        int max_info[9] = {128, 224, 352, 512, 688, 864, 992, 1232, 1456};
        int str_length = strlen(res);
        // Определяем версию QR-кода
        int version = 1;
        for(int i = 0; i < 9; i++) {
            if(max_info[i] < str_length) version = i+2;
        }
        if(str_length > 1456) {
            std::cout << "Data amount is too huge. Program works with qr-code version 1-9 only.\n";
            return;
        }
        std::cout << "Str size: " << str_length << std::endl;
        std::cout << "Version: " << version << std::endl;
        FILE *fp = fopen("qr.png", "wb");
        if (!fp) {
            std::cout << "Can't open file.\n";
            return;
        }
        // Вычисление размеров bmp
        header[22] = header[18] = 17 + 4*version;
        int width = header[18], height = header[22];
        int width_actual = ((width + 31) - (width + 31) % 32) / 8;

        bool **qr_data = new bool*[height];
        for(int i = 0; i < height; i++) {
            qr_data[i] = new bool[width_actual];
        }

        // Вычисление байтов
        int length = height*width_actual;
        header[35] = length >> 8;
        header[34] = length;
        for(int i = 0; i < 62; i++) {
            fwrite(&header[i], sizeof(char), 1, fp);
        }
        int cell_bytes = (width + 7) / 8;
        int last_bit, last_byte;
        int current_pos = 62;
        bool white_colour = true;
        bool number_mode = true;
        auto qr = new unsigned char[length];
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width_actual; j++) {
                qr[i*width_actual + j] = 0x00;
            }
        }

        // Adding search_template
        for (int i = 0; i < 8; i++) {
            char q = search_template[i];
            qr[i*width_actual] = q << 1 | 1;
            qr[(height - i - 1)*width_actual] = q << 1 | 1;
            // Байты правого верхнего угла
            int index1 = (height - i - 1)*width_actual + int((width-8)/8);
            int index2 = (height - i - 1)*width_actual + int(width/8);

            if(version % 2) { // Если число последних значимых битов = 5
                // Вычисляем операнды для изменения рисунка
                // Байт 1: XXXX XAAA
                // Байт 2: BBBB BZZZ
                // Где A и B - пиксели шаблона поиска
                // X - пиксель, относящийся к другой части qr
                // Z - незначимые биты (мусор)
                int and_er1 = search_template[i] >> 5;
                int and_er2 = (search_template[i] << 3) % 256;
                qr[index1] &= 248;
                qr[index1] |= and_er1;
                qr[index2] = and_er2;
            } else { // Если число последних значимых битов = 1
                // Вычисляем операнды для изменения рисунка
                // Байт 1: XAAA AAAA
                // Байт 2: BZZZ ZZZZ
                // Где A и B - пиксели шаблона поиска
                // X - пиксель, относящийся к другой части qr
                // Z - незначимые биты (мусор)
                int and_er1 = search_template[i] >> 1;
                int and_er2 = (search_template[i] << 7) % 256;
                qr[index1] &= 128;
                qr[index1] |= and_er1;
                qr[index2] = and_er2;
            }
        }

        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width_actual; j++) {
                fwrite(&qr[i *width_actual + j], sizeof(char), 1, fp);
            }
        }
        fclose(fp);
        delete[] qr;
        std::cout << "Done." << std::endl;

    }
}