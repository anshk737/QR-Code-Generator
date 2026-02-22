#include "qrcode.h"

void QRCode::build_qr_code(string message, enum EC_LEVEL ec_level, int ver) {
    int dim = 21 + 4*(ver-1);
    int num_tracker = ver < 2 ? 0 : ver/7 +1;

    int codewords = dim*dim - 64*3 - 15 - 16;
    if (ver > 1) {
        codewords -= 25*((num_tracker+1)*(num_tracker+1)-3);
    }
    codewords -= 2*(dim - 16);
    if (ver > 6) {
        codewords -= 18*2 - 10*(num_tracker-1);
    }

    codewords = codewords/8;

    int ec_block_size = ec_level_block_size[ec_level][ver];
    int ec_block_num = ec_level_block_num[ec_level][ver];
    int format = get_ec_format(ec_level);

    int extra = 0;
    if (ver > 9) extra = 1;

    if (message.size() + ec_block_size*ec_block_num > codewords-2-extra) {
        cout << "message too long" << endl;
        return;
    }
    
    // initialising qr code
    code = vector<vector<char>>(dim, vector<char>(dim, '*'));

    // add timing strips
    write_timing_strips();

    //adding markers
    create_marker();
    if (ver > 1) create_tracker(num_tracker, calc_tracker(ver));

    // adding version info
    if (ver > 6) {
        write_version_data(ver);
    }

    // format data  
    write_format_data(create_format_data(format, 2));

    vector<unsigned char> generator = create_generator(ec_block_size);
    vector<unsigned char> bit_stream = bit_stream_data(message, codewords-ec_block_num*ec_block_size, ver);
    vector<unsigned char> ec_bit_stream_data = ec_bit_stream(bit_stream, generator, ec_block_num, ec_block_size);

    write_data(ec_bit_stream_data);
    save_as_bmp("qrcode.bmp");

};

void QRCode::create_tracker(int num, vector<int> pos) {
    vector<string> tracker = {
        "`````",
        "`000`",
        "`0`0`",
        "`000`",
        "`````"
    };

    for (int i = 0; i < pos.size(); ++i) {
        for (int j = 0; j < pos.size(); ++j) {
            if ((i == 0 && j == 0) || (i == 0 && j == pos.size()-1) || (i == pos.size()-1 && j == 0)) continue;

            for (int r = 0; r < 5; ++r) {
                for (int c = 0; c < 5; ++c) {
                    code[pos[i]+r-2][pos[j]+c-2] = tracker[r][c];
                }
            }
        }
    }

};

vector<int> QRCode::calc_tracker(int ver) {
    if (ver <= 1) return {};

    // Using a fixed table to be 100% correct
    int ALIGN_TABLE[41][7] = {
        {}, // Ver 0
        {}, // Ver 1
        {6, 18},                                    // Ver 2
        {6, 22},                                    // Ver 3
        {6, 26},                                    // Ver 4
        {6, 30},                                    // Ver 5
        {6, 34},                                    // Ver 6
        {6, 22, 38},                                // Ver 7
        {6, 24, 42},                                // Ver 8
        {6, 26, 46},                                // Ver 9
        {6, 28, 50},                                // Ver 10
        {6, 30, 54},                                // Ver 11
        {6, 32, 58},                                // Ver 12
        {6, 34, 62},                                // Ver 13
        {6, 26, 46, 66},                            // Ver 14
        {6, 26, 48, 70},                            // Ver 15
        {6, 26, 50, 74},                            // Ver 16
        {6, 30, 54, 78},                            // Ver 17
        {6, 30, 56, 82},                            // Ver 18
        {6, 30, 58, 86},                            // Ver 19
        {6, 34, 62, 90},                            // Ver 20
        {6, 28, 50, 72, 94},                        // Ver 21
        {6, 26, 50, 74, 98},                        // Ver 22
        {6, 30, 54, 78, 102},                       // Ver 23
        {6, 28, 54, 80, 106},                       // Ver 24
        {6, 32, 58, 84, 110},                       // Ver 25
        {6, 30, 58, 86, 114},                       // Ver 26
        {6, 34, 62, 90, 118},                       // Ver 27
        {6, 26, 50, 74, 98, 122},                   // Ver 28
        {6, 30, 54, 78, 102, 126},                  // Ver 29
        {6, 26, 52, 78, 104, 130},                  // Ver 30
        {6, 30, 56, 82, 108, 134},                  // Ver 31
        {6, 34, 60, 86, 112, 138},                  // Ver 32
        {6, 30, 58, 86, 114, 142},                  // Ver 33
        {6, 34, 62, 90, 118, 146},                  // Ver 34
        {6, 30, 54, 78, 102, 126, 150},             // Ver 35
        {6, 24, 50, 76, 102, 128, 154},             // Ver 36
        {6, 28, 54, 80, 106, 132, 158},             // Ver 37
        {6, 32, 58, 84, 110, 136, 162},             // Ver 38
        {6, 26, 54, 82, 110, 138, 166},             // Ver 39
        {6, 30, 58, 86, 114, 142, 170}              // Ver 40
    };

    int numAlign = ver / 7 + 2;
    vector<int> res;
    for (int i = 0; i < numAlign; ++i) {
        res.push_back(ALIGN_TABLE[ver][i]);
    }
    return res;
}

void QRCode::create_marker() {
    vector<string> marker = {
        "000000000",
        "0```````0",
        "0`00000`0",
        "0`0```0`0",
        "0`0```0`0",
        "0`0```0`0",
        "0`00000`0",
        "0```````0",
        "000000000"
    };

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            code[i][j] = marker[i+1][j+1];
            code[i][code.size()-1-j] = marker[i+1][7-j];
            code[code.size()-1-i][j] = marker[7-i][j+1];
        }
    }

};

void QRCode::write_data(vector<unsigned char> data) {

    int s = code.size();
    int bit_count = 7;
    int byte_count = 0;
    bool up = true;

    for (int c = s-1; c >= 0; c -= 2) {
        if (c == 6) c--;
        
        for (int i = 0; i < s; ++i) {

            int row = up ? s-i-1 : i;

            for (int j = 0; j < 2; ++j) {
                int col = c - j;
                
                if (code[row][col] == '*') {
                    
                    if (bit_count < 0) {
                        bit_count = 7;
                        byte_count++;
                    }
                    bool val = false;
                    if (byte_count < data.size()) {

                        
                        val = (data[byte_count] >> bit_count)&1;
                        bit_count--;

                    }
                    //if ((row+col)%2 == 0) val = !val;
                    if (col%3 == 0) val = !val;
                    //if ((row/2 + col/3)%2 == 0) val = !val;

                    code[row][col] = val ? '`' : 'X';

                    
                }
            }

        }

        up = !up;
    }

    cout << "Done!" << endl;
    

};

void QRCode::write_version_data(int ver) {
    int temp = ver << 12;
    
    for (int i = 0; i < 6; ++i) {
        if ((temp >> (17-i))&1) {
            temp ^= (7973 << (5-i));
        }
    }

    int ver_info = (ver << 12) | temp;

    for (int i = 0; i < 18; ++i) {
        char val = (ver_info >> 17-i)&1 ? '`' : 'X';
        code[code.size()-9 - i%3][5 - i/3] = val;
        code[5 - i/3][code.size()-9 - i%3] = val;
    }
    
}

void QRCode::write_format_data(int format) {
    int dim = code.size();
    int bit_count = 14;

    // top-left bottom
    for (int i = 0; i < 9; ++i) {
        if (code[8][i] == '*') {
            code[8][i] = (format >> bit_count)&1 ? '`' : '0';   
            bit_count--;
        }

    }

    //top-left right
    for (int i = 7; i >= 0; --i) {
        if (code[i][8] == '*') {
            code[i][8] = (format >> bit_count)&1 ? '`' : '0';
            bit_count--;
        }
    }

    bit_count = 0;

    // top-right bottom
    for (int i = 0; i < 8; ++i) {
        if (code[8][dim-1-i] == '*') {
            code[8][dim-1-i] = (format >> bit_count)&1 ? '`' : '0';
            bit_count++;
        }
        
    }

    // bottom-left right
    for (int i = 0; i < 8; ++i) {
        if (code[i+dim-8][8] == '*') {
            code[dim-8+i][8] = (format >> bit_count)&1 ? '`' : '0';
            bit_count++;
        }
        
    }

}

int QRCode::get_ec_bytes(enum EC_LEVEL ec_level, int ver) {
    int x = ec_level_block_size[ec_level][ver];
    return x;
}

int QRCode::get_ec_format(enum EC_LEVEL ec_level) {
    vector<int> format = {1,0,3,2};
    return format[ec_level];
}

void QRCode::write_timing_strips() {
    for (int i = 8; i < code.size()-8; ++i) {
        code[i][6] = (i%2 == 0) ? '`' : 'X'; 
        code[6][i] = (i%2 == 0) ? '`' : 'X';
    }

    // setting fixed bit
    code[code.size()-8][8] = '`';
}

int QRCode::create_format_data(int ec_level, int mask) {
    int info = (ec_level << 3) + mask;
    int temp = info << 10;
    
    
    for (int i = 0; i < 5; ++i) {

        if ((temp >> (14-i))&1) {
            temp ^= (1335 << (4-i));
        }
    }

    temp &= 1023;
    int format = (info  << 10) | temp;
    format ^= 21522; 
    return format;
}

unsigned char QRCode::gf_mult(unsigned char x_in, unsigned char y) {
    int res = 0;
    unsigned char x = x_in;
    for (int i = 0; i < 8; ++i) {
        if (y&1) res ^= x;
        bool h = (x&0x80);
        x <<= 1;
        if (h) x ^= 0x11D;
        y >>= 1;
    }

    return (unsigned char)res;
};

vector<unsigned char> QRCode::poly_mult(vector<unsigned char>& a, vector<unsigned char>& b) {
    vector<unsigned char> res(a.size()+b.size()-1, 0);
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            unsigned char temp = gf_mult(a[i], b[j]);
            res[i+j] ^= temp;
        }
    }
    return res;
};

vector<unsigned char> QRCode::synth_div(vector<unsigned char> val, vector<unsigned char> divisor) {
    for (int i = 0; i < val.size()-divisor.size()+1; ++i) {
        unsigned char lead = val[i];
        if (lead != 0) {
            for (int j = 0; j < divisor.size(); ++j) {
                val[i+j] ^= gf_mult(divisor[j], lead);
            }
        }
    }

    vector<unsigned char> remainder(val.end()-int(divisor.size())+1, val.end());

    return remainder;
};

vector<unsigned char> QRCode::create_generator(int len) {
    vector<unsigned char> gen = {1};
    unsigned char root = 1;
    for (int i = 0; i < len; ++i) {
        vector<unsigned char> temp = {1, root};
        gen = poly_mult(gen, temp);
        root = gf_mult(root, 2);
    }

    return gen;
}

vector<unsigned char> QRCode::bit_stream_data(string data, int max_cap, int ver) {
    int data_len = data.size();

    // Creating header
    
    vector<unsigned char> temp = {4};
    
    if (ver < 10) {
        temp.push_back(data_len >> 4);
        temp.push_back(data_len&15);
    } else {
        temp.push_back(data_len >> 12);
        temp.push_back((data_len >> 8)&15);
        temp.push_back((data_len >> 4)&15);
        temp.push_back(data_len&15);
    }
    
    for (char c : data) {
        temp.push_back(c >> 4);
        temp.push_back(c&15);
    }

    if (temp.size()%2 != 0) temp.push_back(0);

    vector<unsigned char> bit_stream;

    for (int i = 0; i < temp.size(); i += 2) {
        unsigned char x = (temp[i] << 4) + temp[i+1];
        bit_stream.push_back(x);
    }

    bool flip = true;
    for (int i = bit_stream.size(); i < max_cap; ++i) {
        bit_stream.push_back(flip ? 236 : 17);
        flip = !flip;
    }

    return bit_stream;
};

vector<unsigned char> QRCode::ec_bit_stream(vector<unsigned char> bit_stream, vector<unsigned char> generator, int ec_block_num, int ec_block_size) {
    vector<vector<unsigned char>> data;
    vector<vector<unsigned char>> ec_data;

    int num_total_blocks = ec_block_num;
    int total_data_codewords = bit_stream.size();
    
    int num_short_blocks = num_total_blocks - (total_data_codewords % num_total_blocks);
    int num_long_blocks = num_total_blocks - num_short_blocks;
    
    int short_block_len = total_data_codewords / num_total_blocks;
    int long_block_len = short_block_len + 1;
    
    int prev = 0;
    for (int i = 0; i < num_total_blocks; ++i) {
        int curr_size = (i < num_short_blocks) ? short_block_len : long_block_len;

        vector<unsigned char> poly;
        for (int j = 0; j < curr_size; ++j) {
            poly.push_back(bit_stream[prev++]);
        }

        data.push_back(poly);

        // Generate EC codewords for curr block
        vector<unsigned char> poly_for_div = poly;
        for (int j = 0; j < ec_block_size; ++j) poly_for_div.push_back(0);
        vector<unsigned char> rem = synth_div(poly_for_div, generator);

        ec_data.push_back(rem);
    }
    

    // Interleaving
    vector<unsigned char> res;

    for (int i = 0; i < long_block_len; ++i) {
        for (int j = 0; j < ec_block_num; ++j) {
            if (i < data[j].size()) {
                res.push_back(data[j][i]);
            }
        }
    }

    for (int i = 0; i < ec_block_size; ++i) {
        for (int j = 0; j < ec_block_num; ++j) {
            res.push_back(ec_data[j][i]);
        }
    }

    return res;
}

void QRCode::print_code() {
    for (vector<char> r : code) {
        for (char c : r) {
            cout << c << ' ';
        }
        cout << ' ' << endl;
    }
};

void QRCode::save_as_bmp(const string& filename) {
    int scale = 10; 
    int border = 4;
    int dim = code.size();
    int size = (dim + border * 2) * scale;
    int rowSize = (3 * size + 3) & (~3);
    int fileSize = 54 + rowSize * size;

    unsigned char header[54] = {
        'B','M', (unsigned char)fileSize, (unsigned char)(fileSize>>8), (unsigned char)(fileSize>>16), (unsigned char)(fileSize>>24),
        0,0,0,0, 54,0,0,0, 40,0,0,0,
        (unsigned char)size, (unsigned char)(size>>8), (unsigned char)(size>>16), (unsigned char)(size>>24),
        (unsigned char)size, (unsigned char)(size>>8), (unsigned char)(size>>16), (unsigned char)(size>>24),
        1,0, 24,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };

    ofstream out(filename, ios::binary);
    out.write((char*)header, 54);

    vector<unsigned char> row(rowSize, 255);

    for (int y = 0; y < size; ++y) {
        fill(row.begin(), row.end(), 255);
        
        int qr_row = (size - 1 - y) / scale - border;
        
        for (int x = 0; x < size; ++x) {
            int qr_col = x / scale - border;

            if (qr_row >= 0 && qr_row < dim && qr_col >= 0 && qr_col < dim) {
                if (code[qr_row][qr_col] == '`') {
                    row[x*3] = 0;
                    row[x*3+1] = 0;
                    row[x*3+2] = 0;   
                }
            }
        }
        out.write((char*)row.data(), rowSize);
    }
}
