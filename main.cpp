#include "qrcode.h"

int main(void) {

    QRCode q;
    int ver;
    int ec_level;
    string message;

    cout << "message: " << endl;
    getline(cin, message);

    cout << "version (1 - 18): ";
    cin >> ver;
    cout << "error correction level (0, 1, 2, 3): ";
    cin >> ec_level;
    cout << endl;
    

    q.build_qr_code(message, EC_LEVEL(ec_level), ver);

    return 0;
};