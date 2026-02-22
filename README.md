# QR-Code-Generator
A QR code generator built from scratch in C++.

A C++ project which generators QR code. Although they may seem simple, there is a lot of cool maths behind the scene to makes these QR codes robust and universally readable. 

## Solomon-Reed Encoding
The heart of this project is Solomon-Reed encoding which allows QR codes to be read even if not all of the code is visible.

## ISO/IEC 18004
To make the QR code readable to all devices the data must be placed exactly as defined in the ISO/IEC 18004. The basic anatomy of QR is consists of 3 8x8 trackers on each corner except the bottom right. There are also smaller 5x5 alignment patterns dotted around the code to ensure the scaners can read the data from various angles. There are data strips around the trackers which specify QR code version, masking pattern and error correction level.


