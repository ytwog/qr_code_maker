#include "lab6.h"

namespace Apa {

    unsigned char *append_bits(char *result, char *bits, int bits_length, int last_bit, int last_byte) {
        return nullptr;
    }

    void print_matrixB(bool *res, int h, int w) {
        if(res == nullptr) return;
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j++) {
                //std::cout << int(res[i*w + j]) << ".";
                if(res[i*w + j]) std::cout << "+  ";
                else std::cout << "-  ";
            }
            std::cout << std::endl;
        }
    }

    int char_to_int(char ch) {
        if(ch > '9' || ch < '0') return -1;
        else return ch-48;
    }

    void append_data(bool *qr_res, int &pos, int length, bool *new_data, bool reverse = false) {
        for(int i = 0; i < length; i++) {
            if(reverse) {
                qr_res[pos+i] = new_data[length-1-i];
            } else {
                qr_res[pos+i] = new_data[i];
            }
        }
        pos += length;
    }

    void matrix_to_bytes(bool **ask, int h, int w, unsigned char*res, int length, int height_actual) {
        int index, byte_num, width_actual = length/height_actual;
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
                int pos = 1;
                for(int x = 0; x <= 7-index; x++) {
                    temp_byte |= pos;
                    pos *= 2;
                }
                res[byte_num + width_actual*i] = temp_byte; // Сохраняем в массиве то, что набралось
            }
        }
    }

    void writeBMP(const char* res) {
        if(strlen(res) == 0) {
            std::cout << "String is empty" << std::endl;
            return;
        }
        int size_limit = 34;
        bool temp_array[3][10] = {{}, {}, {0,0,0,0,0,0,0,0,0,0}};

        unsigned char header[62] =   {0x42, 0x4d, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
                                      0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
                                      0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0xff, 0xff, 0xff, 0x00};
        int max_info[9] = {34, 63, 101, 149, 202, 255, 293, 365, 432};
        int alignment_info[9][4] = {
                {0, 0, 0, 0}, {1, 18, 0, 0}, {1, 22, 0, 0},
                {1, 26, 0, 0}, {1, 30, 0, 0}, {1, 34, 0, 0},
                {3, 6, 22, 38}, {3, 6, 24, 42}, {3, 6, 26, 46}
        };
        bool number_mode = true;
        int str_length = strlen(res);
        // Определяем версию QR-кода
        int version = 1;
        for(int i = 0; i < 9; i++) {
            if(max_info[i] < str_length) {
                version = i+2;
                size_limit = max_info[i+1];
            }
        }
        if(str_length > 432) {
            std::cout << "Data amount is too huge. Program works with qr-code version 1-9 only.\n";
            return;
        }
        // Создаем массив потока данных
        int res_pos = 0;
        bool *qr_res = new bool[size_limit*8+50];
        for(int i = 0; i < size_limit*8+50; i++) {
            qr_res[i] = 0;
        }

        // Добавляем Mode indicator
        bool mode[2][4] = {{0,0,0,1}, {0,0,1,0}};
        if(number_mode) {
            append_data(qr_res, res_pos, 4, mode[0]);
        } else {
            append_data(qr_res, res_pos, 4, mode[1]);
        }

        // Добавляем Character Count Indicator (10=number, 9=alphabetical)
        bool cci[10] = {0,0,0,0,0,0,0,0,0,0};
        int temp_count = 0;
        int temp_str_length = str_length;
        while(temp_str_length) {
            cci[temp_count] = temp_str_length % 2;
            temp_count++;
            temp_str_length /= 2;
        }
        // Ставим незначимые нули
        append_data(qr_res, res_pos, 9 + number_mode - temp_count, temp_array[2]);

        append_data(qr_res, res_pos, temp_count, cci, true);

        // Вычисляем encoded data
        if(number_mode) {
            for(int i = 0; i < str_length; i+=3) {
                // Размеры блоков
                short sizes[3] = {4, 7, 10};
                // Массив хранения блоков данных
                bool block[10] = {0,0,0,0,0,0,0,0,0,0};
                temp_count = 0;
                int temp_len = 0;
                temp_str_length = char_to_int(res[i]);
                if(i + 1 < str_length) {
                    temp_len++;
                    temp_str_length = temp_str_length * 10 + char_to_int(res[i + 1]);
                }
                if(i + 2 < str_length) {
                    temp_len++;
                    temp_str_length = temp_str_length * 10 + char_to_int(res[i + 2]);
                }
                std::cout << temp_str_length << "\n";
                while(temp_str_length) {
                    block[temp_count] = temp_str_length % 2;
                    std::cout << temp_str_length%2 << " ";
                    temp_count++;
                    temp_str_length /= 2;
                }
                append_data(qr_res, res_pos, sizes[temp_len] - temp_count, temp_array[2]);
                append_data(qr_res, res_pos, temp_count, block, true);
            }
        }

        std::cout << "Str size: " << str_length << "; " << "Byte size:" << size_limit << std::endl;
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
        // Выделение рамки
        header[22] = header[18] += 16;
        int width_actual_margin = ((header[18] + 31) - (header[18] + 31) % 32) / 8;

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
        int length_margin = header[22]*width_actual_margin;
        header[35] = length_margin >> 8;
        header[34] = length_margin;
        auto qr = new unsigned char[length_margin];
        for(int i = 0; i < header[22]; i++) {
            for(int j = 0; j < width_actual_margin; j++) {
                qr[i*width_actual_margin + j] = 0xff;
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
        for(int i = 1; i <= alignment_info[version-1][0]; i++) {
            int coord_i = alignment_info[version-1][i] - 2;
            for(int j = 1; j <= alignment_info[version-1][0]; j++) {
                bool is_clear = true;
                int coord_j = alignment_info[version-1][j] - 2;
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
        print_matrixB(qr_res, height, width);

        // Размещение основных модулей
        int data_pos = 0;
        bool up_growing = true; // Направление движения потока
        for(int j = width-1; j >=0; j-= 2) { // Поток идет с конца по два модуля
            for(int i = up_growing ? (height-1) : 0;
                up_growing ? (i >= 0) : (i < height); up_growing ? i-- : i++) { // Вверх/вниз
                // Если место для размещения свободно
                if(!qr_final[i][j]) {
                    qr_final[i][j] = 1;
                    qr_data[i][j] = qr_res[data_pos];
                    data_pos++;
                }
                // То же самое для модуля слева
                if(j > 0 && !qr_final[i][j-1]) {
                    qr_final[i][j-1] = 1;
                    qr_data[i][j-1] = qr_res[data_pos];
                    data_pos++;
                }
            }
            up_growing = !up_growing; // Меняем направление
        }
        matrix_to_bytes(qr_data, height, width, qr, length_margin, header[22]);
        for(int i = 0; i < 62; i++) {
            fwrite(&header[i], sizeof(char), 1, fp);
        }
        // Рамка сверху
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < width_actual_margin; j++) {
                unsigned char margin_byte = 0xff;
                fwrite(&margin_byte, sizeof(char), 1, fp);
            }
        }
        for(int i = height-1; i >= 0; i--) {
            unsigned char margin_byte = 0xff;
            fwrite(&margin_byte, sizeof(char), 1, fp);
            for(int j = 0; j < width_actual_margin-1; j++) {
                fwrite(&qr[i *width_actual_margin + j], sizeof(char), 1, fp);
            }
        }
        // Рамка снизу
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < width_actual_margin; j++) {
                unsigned char margin_byte = 0xff;
                fwrite(&margin_byte, sizeof(char), 1, fp);
            }
        }
        fclose(fp);
        for(int i = 0; i < height; i++) {
            delete[] qr_final[i];
            delete[] qr_data[i];
        }
        delete[] qr_data;
        delete[] qr_final;
        delete[] qr_res;
        delete[] qr;
        std::cout << "Done." << std::endl;

    }
}