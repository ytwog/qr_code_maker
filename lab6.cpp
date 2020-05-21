#include "lab6.h"

namespace Apa {

    bool version_info[3][18] = {
            {0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,1,0,0},
            {0,0,1,0,0,0,0,1,0,1,1,0,1,1,1,1,0,0},
            {0,0,1,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1}
    };

    char alphabet[45] = {
            '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E',
            'F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T',
            'U','V','W','X','Y','Z',' ','$','%','*','+','-','.','/',':'
    };

    int index_of(char ch) {
        for(int i = 0; i < 45; i++) {
            if(alphabet[i] == ch) return i;
        }
        return -1;
    }

    bool determine_mode(const char* res, bool &ok) {
        bool ans = true;
        for(int i = 0; i < strlen(res); i++) {
            if(index_of(res[i]) == -1) {
                ok = false;
                return false;
            } else {
                if(res[i] > '9' || res[i] < '0') {
                    ans = false;
                }
            }
        }
        return ans;
    }

    int galua[255] = {
            1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,
            76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,
            157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,
            70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,
            95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,
            253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,
            217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,
            129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,
            133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,
            168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,
            230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,
            227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,
            130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,
            81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,
            18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,
            44,88,176,125,250,233,207,131,27,54,108,216,173,71,142
    };

    int galua_reverse[255] = {
            -1,0,1,25,2,50,26,198,3,223,51,238,27,104,199,75,
            4,100,224,14,52,141,239,129,28,193,105,248,200,8,76,113,
            5,138,101,47,225,36,15,33,53,147,142,218,240,18,130,69,
            29,181,194,125,106,39,249,185,201,154,9,120,77,228,114,166,
            6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,
            54,208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,
            30,66,182,163,195,72,126,110,107,58,40,84,250,133,186,61,
            202,94,155,159,10,21,121,43,78,212,229,172,115,243,167,87,
            7,112,192,247,140,128,99,13,103,74,222,237,49,197,254,24,
            227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,
            55,63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,
            242,86,211,171,20,42,93,158,132,60,57,83,71,109,65,162,
            31,45,67,216,183,123,164,118,196,23,73,236,127,12,111,246,
            108,161,59,82,41,157,85,170,251,96,134,177,187,204,62,90,
            203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,
            79,174,213,233,230,231,173,232,116,214,244,234,168,80,88
    };

    int take_byte(bool *res, int &pos) {
        int ans = 0;
        for(int i = 0; i < 8; i++) {
            ans = ans*2 + res[pos+i];
        }
        pos += 8;
        return ans;
    }

    bool is_mask_true(int row, int column) {
        return ((row + column) % 2) == 0;
    }

    void print_matrixB(bool **res, int h, int w) {
        if(res == nullptr) return;
        for(int i = 0; i < h; i++) {
            for(int j = 0; j < w; j++) {
                //std::cout << int(res[i*w + j]) << ".";
                if(res[i][j]) std::cout << "+  ";
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

    void byte_to_bitarray(bool *res, int &pos, unsigned char byte) {
        for(int i = 7; i >= 0; i--) {
            res[pos+i] = byte % 2;
            //std::cout << (res[pos+i] ? "+" : "-") << " ";
            byte /= 2;
        }
        //std::cout << std::endl;
        pos += 8;
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
        int size_limit = 16;
        bool temp_array[3][10] = {{1,1,1,0,1,1,0,0}, {0,0,0,1,0,0,0,1}, {0,0,0,0,0,0,0,0,0,0}};

        int er_byte_10[10] = {251, 67, 46, 61, 118, 70, 64, 94, 32, 45};
        int er_byte_16[16] = {120, 104, 107, 109, 102, 161, 76, 3, 91, 191, 147, 169, 182, 194, 225, 120};
        int er_byte_18[18] = {215, 234, 158, 94, 184, 97, 118, 170, 79, 187, 152, 148, 252, 179, 5, 98, 96, 153};
        int er_byte_22[22] = {210, 171, 247, 242, 93, 230, 14, 109, 221, 53, 200, 74, 8,
                              172, 98, 80, 219, 134, 160, 105, 165, 231};
        int er_byte_24[24] = {229, 121, 135, 48, 211, 117, 251, 126, 159, 180, 169, 152, 192, 226, 228, 218, 111,
                              0, 117, 232, 87, 96, 227, 21};
        int er_byte_26[26] = {173, 125, 158, 2, 103, 182, 118, 17, 145, 201, 111, 28, 165, 53, 161, 21, 245,
                              142, 13, 102, 48, 227, 153, 145, 218, 70};

        unsigned char header[62] =   {0x42, 0x4d, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e,
                                      0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
                                      0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                      0x00, 0x00, 0xff, 0xff, 0xff, 0x00};
        int block_count[9] = {1, 1, 1, 2, 2, 4, 4, 4, 5};
        int block_reminder, block_divided;
        int max_info[9] = {128,224,352,512,688,864,992,1232,1456};
        int data_size[2][9] = {
                {34, 63, 101, 149, 202, 255, 293, 365, 432},
                {20, 38, 61, 90, 122, 154, 178, 221, 262}
        };
        int max_info_ec[9] = {10,16,26,18,24,16,18,22,22};
        int alignment_info[9][4] = {
                {0, 0, 0, 0}, {1, 18, 0, 0}, {1, 22, 0, 0},
                {1, 26, 0, 0}, {1, 30, 0, 0}, {1, 34, 0, 0},
                {3, 6, 22, 38}, {3, 6, 24, 42}, {3, 6, 26, 46}
        };

        bool ok = true;
        bool number_mode = determine_mode(res, ok);
        if(!ok)
            return;
        if(number_mode) std::cout << "NUMBER MODE" << std::endl;
        else std::cout << "ALPHABETIC MODE" << std::endl;
        int str_length = strlen(res);
        // Определяем версию QR-кода
        int version = 1;
        for(int i = 0; i < 9; i++) {
            if(str_length > data_size[number_mode ? 0 : 1][i]) {
                version = i+2;
                size_limit = max_info[i+1] / 8;
            }
        }
        if(str_length > 432) {
            std::cout << "Data amount is too huge. Program works with qr-code version 1-9 only.\n";
            return;
        }

        int er_byte[30];
        // Заполняем массив со сгенерированными полиномами
        for(int j = 0; j < max_info_ec[version-1]; j++) {
            switch(version) {
                case 1:
                    er_byte[j] = er_byte_10[j];
                    break;
                case 2:
                case 6:
                    er_byte[j] = er_byte_16[j];
                    break;
                case 3:
                    er_byte[j] = er_byte_26[j];
                    break;
                case 4:
                case 7:
                    er_byte[j] = er_byte_18[j];
                    break;
                case 5:
                    er_byte[j] = er_byte_24[j];
                    break;
                case 8:
                case 9:
                    er_byte[j] = er_byte_22[j];
                    break;
            }
        }
        // Создаем массив потока данных
        int res_pos = 0;
        bool *qr_res = new bool[size_limit*8*3];
        for(int i = 0; i < size_limit*8*3; i++) {
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
        } else {
            for(int i = 0; i < str_length; i+=2) {
                temp_count = 0;
                // Массив хранения блоков данных
                bool block[11] = {0,0,0,0,0,0,0,0,0,0};
                bool full_data = (i != str_length-1);
                int q1 = index_of(res[i]);
                if(full_data)
                    q1 = q1*45 + index_of(res[i+1]);
                while(q1) {
                    block[temp_count] = q1 % 2;
                    std::cout << q1%2 << " ";
                    temp_count++;
                    q1 /= 2;
                }
                append_data(qr_res, res_pos, 6 + 5*full_data - temp_count, temp_array[2]);
                append_data(qr_res, res_pos, temp_count, block, true);
            }
        }
        std::cout << std::endl;
        // Добавляет терминатор строки 0000
        int bits_left = 8*size_limit - res_pos;
        append_data(qr_res, res_pos, std::fmin(bits_left, 4), temp_array[2]);
        bits_left -= 4;

        // Если последний байт неокончен, дополняем его нулями
        int left_to_byte = std::fmin(bits_left, (8 - res_pos%8)%8);
        append_data(qr_res, res_pos, left_to_byte, temp_array[2]);
        bits_left -= left_to_byte;
        // Если ещё есть пространство, заполняем его байтами 236 и 17.
        while(bits_left > 0) {
            append_data(qr_res, res_pos, std::fmin(bits_left, 8), temp_array[0]);
            bits_left = 8*size_limit - res_pos;
            if(bits_left < 1) break;
            append_data(qr_res, res_pos, std::fmin(bits_left, 8), temp_array[1]);
            bits_left = 8*size_limit - res_pos;
        }

        // Считает число байтов в минимальных блоках
        int block_already_taken = 0;
        block_divided = size_limit / block_count[version-1];
        block_reminder = size_limit % block_count[version-1];
        // Заполняет блоки данных и вычисляет блоки коррекции
        auto *blocks_da = new std::vector <int>[block_count[version-1]];
        auto *blocks_ec = new std::vector <int>[block_count[version-1]];
        for(int i = 0; i < block_count[version-1]; i++) {
            bool add_one = (i >= (block_count[version-1]-block_reminder) ? 1 : 0);
            // Длина блока
            int lenlen = std::fmax(block_divided+add_one, max_info_ec[version-1]);

            // Заполняем нулями блоки
            for(int j = 0; j < lenlen; j++) {
                blocks_ec[i].push_back(0);
            }
            // Заполняем блок
            for(int j = 0; j < block_divided+add_one; j++) {
                blocks_ec[i][j] = take_byte(qr_res, block_already_taken);
                blocks_da[i].push_back(blocks_ec[i][j]);
            }

            // Основной цикл обработки
            for(int j = 0; j < block_divided+add_one; j++) {
                int varA = blocks_ec[i][0];
                blocks_ec[i].erase(blocks_ec[i].begin());
                blocks_ec[i].push_back(0);
                if(varA == 0) {
                    continue;
                }
                int varB = galua_reverse[varA];
                for(int z = 0; z < max_info_ec[version-1]; z++) {
                    int varC = (er_byte[z] + varB) % 255;
                    blocks_ec[i][z] ^= galua[varC];
                }
            }
        }

        std::cout << "-------------------------------" << std::endl;
        std::cout << "Data bytes: " << std::endl;
        for(int i = 0; i < block_count[version-1]; i++) {
            std::cout << "block #: " << i << std::endl;
            for (auto q: blocks_da[i]) {
                std::cout << "block = " << q << std::endl;
            }
        }
        std::cout << "-------------------------------" << std::endl;
        std::cout << "Error Correction bytes: " << std::endl;
        for(int i = 0; i < block_count[version-1]; i++) {
            std::cout << "block #: " << i << std::endl;
            for (auto q: blocks_ec[i]) {
                std::cout << "block = " << q << std::endl;
            }
        }
        std::cout << "-------Blocks are connecting.------" << std::endl;
        int blocks_left = block_count[version-1];
        int final_pos = 0;
        bool final_data[4096];
        while(blocks_left) {
            for (int i = 0; i < block_count[version - 1]; i++) {
                if(blocks_da[i].empty()) continue;
                byte_to_bitarray(final_data, final_pos, blocks_da[i][0]);
                std::cout << blocks_da[i][0] << " ";
                blocks_da[i].erase(blocks_da[i].begin());
                if(blocks_da[i].empty())
                    blocks_left--;
            }
        }
        std::cout << "Now connect EC blocks" << std::endl;
        for(int b = 0; b < max_info_ec[version-1]; b++) {
            for (int i = 0; i < block_count[version - 1]; i++) {
                byte_to_bitarray(final_data, final_pos, blocks_ec[i][b]);
                std::cout << blocks_ec[i][b] << " ";
            }
        }
        std::cout << std::endl;
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

        // Добавляем один черный модуль
        qr_data[height-8][8] = 0;
        qr_final[height-8][8] = 1;

        // Индикатор маски слева сверху
        //bool mask_bits[15] = {1,1,1,1,0,0,0,1,0,0,1,1,1,0,1};
        bool mask_bits[15] = {1,0,1,0,1,0,0,0,0,0,1,0,0,1,0};
        for(int i = 0; i < 15; i++) {
            if(i < 8) {
                qr_data[8][i+(i>5)] = !mask_bits[i];
                qr_final[8][i+(i>5)] = 1;
            } else {
                qr_data[15-i-(i>8)][8] = !mask_bits[i];
                qr_final[15-i-(i>8)][8] = 1;
            }
        }
        // Индикатор маски #2
        for(int i = 0; i < 15; i++) {
            if(i < 7) {
                qr_data[height-i-1][8] = !mask_bits[i];
                qr_final[height-i-1][8] = 1;
            } else {
                qr_data[8][width-15+i] = !mask_bits[i];
                qr_final[8][width-15+i] = 1;
            }
        }
        // Индикатор версии
        if(version > 6) {
            for(int i = 0; i < 6; i++) {
                for(int j = 0; j < 3; j++) {
                    qr_data[height-9-j][5-i] = !version_info[version-7][i*3+j];
                    qr_final[height-9-j][5-i] = 1;
                }
            }
            for(int i = 0; i < 6; i++) {
                for(int j = 0; j < 3; j++) {
                    qr_data[5-i][width-9-j] = !version_info[version-7][i*3+j];
                    qr_final[5-i][width-9-j] = 1;
                }
            }
        }

        // Размещение основных модулей
        int data_pos = 0;
        bool up_growing = true; // Направление движения потока
        for(int j = width-1; j >=0; j-= 2) { // Поток идет с конца по два модуля
            for(int i = up_growing ? (height-1) : 0;
                up_growing ? (i >= 0) : (i < height); up_growing ? i-- : i++) { // Вверх/вниз
                // По идее эта строка фиксит "исключение", но это не точно
                if(j == 6) j-=1;
                // Если место для размещения свободно
                if(!qr_final[i][j]) {
                    //qr_final[i][j] = 1;
                    qr_data[i][j] = final_data[data_pos];
                    if(!is_mask_true(i,j)) qr_data[i][j] = !qr_data[i][j];
                    data_pos++;
                }
                // То же самое для модуля слева
                if(j == 7) {
                    if (j > 0 && !qr_final[i][j - 2]) {
                        //qr_final[i][j - 2] = 1;
                        qr_data[i][j - 2] = final_data[data_pos];
                        if(!is_mask_true(i,j-2)) qr_data[i][j-2] = !qr_data[i][j-2];
                        data_pos++;
                    }
                }
                else {
                    if (j > 0 && !qr_final[i][j - 1]) {
                        //qr_final[i][j - 1] = 1;
                        qr_data[i][j - 1] = final_data[data_pos];
                        if(!is_mask_true(i,j-1)) qr_data[i][j-1] = !qr_data[i][j-1];
                        data_pos++;
                    }
                }
            }
            up_growing = !up_growing; // Меняем направление
        }



        matrix_to_bytes(qr_data, height, width, qr, length_margin, header[22]);
        print_matrixB(qr_data, height, width);

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
        delete[] blocks_ec;
        delete[] qr_res;
        delete[] qr;
        std::cout << "Done." << std::endl;

    }
}