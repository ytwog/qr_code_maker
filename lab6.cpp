#include "lab6.h"

namespace Apa {

    unsigned char *append_bits(char *result, char *bits, int bits_length, int last_bit, int last_byte) {
        return nullptr;
    }

    void print_matrixB(bool **res, int h, int w) {
        if(res == nullptr) return;
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j++) {
                if(res[i][j]) std::cout << "+  ";
                else std::cout << "-  ";
            }
            std::cout << std::endl;
        }
    }

    void matrix_to_bytes(bool **ask, int h, int w, unsigned char*res, int length) {
        int index, byte_num, width_actual = length/h;
        unsigned char temp_byte;
        if(res == nullptr || ask == nullptr) return;
        for(int i = 0; i < h; i++) {
            byte_num = 0;
            index = 0;
            temp_byte = 0; // Заполняем байт
            for(int j = 0; j < w; j++) {
                index++; // Номер бита
                if(ask[i][j]) temp_byte |= 1;
                if(index == 8) { // Если байт собран
                    index = 0;
                    res[byte_num + width_actual*i] = temp_byte; // Сохраняем в массиве
                    temp_byte = 0;
                    byte_num++;
                } else {
                    temp_byte <<= 1;
                }
            }
            if(index != 0) { // Если строка оборвалась, не набрав байта
                temp_byte <<= 7-index;
                res[byte_num + width_actual*i] = temp_byte; // Сохраняем в массиве то, что набралось
            }
        }
    }

    void writeBMP(const char* res) {

        unsigned char header[62] =   {0x42, 0x4d, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
                                      0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
                                      0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0xff, 0xff, 0xff, 0x00};
        int max_info[9] = {128, 224, 352, 512, 688, 864, 992, 1232, 1456};
        int alignment_info[9][4] = {
                {0, 0, 0, 0}, {1, 18, 0, 0}, {1, 22, 0, 0},
                {1, 26, 0, 0}, {1, 30, 0, 0}, {1, 34, 0, 0},
                {3, 6, 22, 38}, {3, 6, 24, 42}, {3, 6, 26, 46}
        };
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

        // Массив рисунка QR-кода
        bool **qr_data = new bool*[height];
        for(int i = 0; i < height; i++) {
            qr_data[i] = new bool[width];
        }

        // Массив подтвержденных модулей
        bool **qr_final = new bool*[height];
        for(int i = 0; i < height; i++) {
            qr_final[i] = new bool[width];
            for(int j = 0; j < width; j++)
                qr_final[i][j] = 0;
        }

        // Вычисление байтов
        int length = height*width_actual;
        header[35] = length >> 8;
        header[34] = length;
        int cell_bytes = (width + 7) / 8;
        int last_bit, last_byte;
        int current_pos = 62;
        bool white_colour = true;
        bool number_mode = true;
        auto qr = new unsigned char[length];
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width_actual; j++) {
                qr[i*width_actual + j] = 0xff;
            }
        }

        // Создаем шаблон поиска
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++) {
                // Слева сверху
                qr_final[i][j] = 1;
                if((i == 0 || i == 6) && (j != 7)) qr_data[i][j] = 0;
                else if((j == 0 || j == 6) && (i != 7)) qr_data[i][j] = 0;
                else if(i > 1 && i < 5 && j > 1 && j < 5) qr_data[i][j] = 0;
                else qr_data[i][j] = 1;
                // Снизу слева
                qr_final[height-8+i][j] = 1;
                if((i == 1 || i == 7) && (j != 7)) qr_data[height-8+i][j] = 0;
                else if((j == 0 || j == 6) && (i != 0)) qr_data[height-8+i][j] = 0;
                else if(i > 2 && i < 6 && j > 1 && j < 5) qr_data[height-8+i][j] = 0;
                else qr_data[height-8+i][j] = 1;
                // Справа сверху
                qr_final[i][width-8+j] = 1;
                if((j == 1 || j == 7) && (i != 7)) qr_data[i][width-8+j] = 0;
                else if((i == 0 || i == 6) && (j != 0)) qr_data[i][width-8+j] = 0;
                else if(j > 2 && j < 6 && i > 1 && i < 5) qr_data[i][width-8+j] = 0;
                else qr_data[i][width-8+j] = 1;
            }
        // Создание узоров распознавания
        for(int i = 1; i <= alignment_info[version][0]; i++) {
            int coord_i = alignment_info[version][i] - 2;
            for(int j = 1; j <= alignment_info[version][0]; j++) {
                bool is_clear = true;
                int coord_j = alignment_info[version][j] - 2;
                for (int i1 = 0; i1 < 5; i1++) {
                    for (int i2 = 0; i2 < 5; i2++) {
                        if(qr_final[coord_i+i1][coord_j+i2]) {
                            is_clear = false;
                        }
                    }
                }
                if(is_clear) {
                    for (int i1 = 0; i1 < 5; i1++) {
                        for (int i2 = 0; i2 < 5; i2++) {
                            qr_final[coord_i+i1][coord_j+i2] = 1;
                            if(i1 == 0 || i1 == 4 || i2 == 0 || i2 == 4 || (i1 == 2 && i2 == 2))
                                qr_data[coord_i+i1][coord_j+i2] = 0;
                            else
                                qr_data[coord_i+i1][coord_j+i2] = 1;
                        }
                    }
                }
            }
        }
        // Создание timing pattern
        for(int i = 8; i < height-8; i++) {
            // Слева
            qr_final[i][6] = 1;
            if (i % 2) qr_data[i][6] = 1;
            else qr_data[i][6] = 0;
            // Сверху
            qr_final[6][i] = 1;
            if (i % 2) qr_data[6][i] = 1;
            else qr_data[6][i] = 0;
        }
        matrix_to_bytes(qr_data, height, width, qr, length);
        for(int i = 0; i < 62; i++) {
            fwrite(&header[i], sizeof(char), 1, fp);
        }
        for(int i = height-1; i >= 0; i--) {
            for(int j = 0; j < width_actual; j++) {
                fwrite(&qr[i *width_actual + j], sizeof(char), 1, fp);
            }
        }
        fclose(fp);
        for(int i = 0; i < height; i++) {
            delete[] qr_data[i];
            delete[] qr_final[i];
        }
        delete[] qr_data;
        delete[] qr_final;
        delete[] qr;
        std::cout << "Done." << std::endl;

    }
}