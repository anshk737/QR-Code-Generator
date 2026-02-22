#ifndef QRCODE_H
#define QRCODE_H

#include <bits/stdc++.h>
using namespace std;

enum EC_LEVEL {
    LOW,
    MEDIUM,
    QUARTILE,
    HIGH
};

class QRCode {
    private:
        // Using fixed value for ISO
        vector<vector<int>> ec_level_block_size = {
        	{0,  7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Low
            {0, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28},  // Medium
            {0, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // Quartile
            {0, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},  // High
        };

        vector<vector<int>> ec_level_block_num = {
            {0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4,  4,  4,  4,  4,  6,  6,  6,  6,  7,  8,  8,  9,  9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25},  // Low
            {0, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5,  5,  8,  9,  9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49},  // Medium
            {0, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8,  8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68},  // Quartile
            {0, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81},  // High
        };

        vector<vector<char>> code;

    public:
        QRCode() {};

        void build_qr_code(string message, enum EC_LEVEL ec_level, int ver);

        void create_tracker(int num, vector<int> pos);

        vector<int> calc_tracker(int ver);

        void create_marker();

        void write_data(vector<unsigned char> data);

        void write_format_data(int format);
        
        void write_version_data(int ver);

        int get_ec_bytes(enum EC_LEVEL ec_level, int ver);

        int get_ec_format(enum EC_LEVEL ec_level);

        void write_timing_strips();

        int create_format_data(int ec_level, int mask);

        unsigned char gf_mult(unsigned char x_in, unsigned char y);

        vector<unsigned char> poly_mult(vector<unsigned char>& a, vector<unsigned char>& b);

        vector<unsigned char> synth_div(vector<unsigned char> val, vector<unsigned char> divisor);

        vector<unsigned char> create_generator(int len);

        vector<unsigned char> bit_stream_data(string data, int max_cap, int ver);

        vector<unsigned char> ec_bit_stream(vector<unsigned char> bit_stream, vector<unsigned char> generator, int ec_block_num, int ec_block_size);

        // for debugging
        void print_code();

        void save_as_bmp(const string& filename);

};

#endif