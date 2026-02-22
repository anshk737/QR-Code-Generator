# QR Code Generator

![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)
![OS](https://img.shields.io/badge/OS-Linux-orange.svg)

A C++ application that generates QR codes from scratch. This project handles everything from bit-stream generation to Reed-Solomon error correction.

## 📋 Table of Contents
- [Features](#-features)
- [Project Structure](#-project-structure)
- [Installation & Compilation](#-installation--compilation)

---

## ✨ Features
- **Custom QR Logic:** Manual implementation of QR encoding (Versions 1-18).
- **Error Correction:** Built-in Reed-Solomon error correction logic.

## 📂 Project Structure
- `main.cpp`: Take input from user.
- `qrcode.cpp` / `qrcode.h`: The core QR generation and bit-masking logic.
- `Makefile`: Optimized build script for Linux.

## 🚀 Installation & Compilation
1. Clone your repository:
2. Compile using Make:
```bash
make
```
3. Run `app`
```bash
./app
```
