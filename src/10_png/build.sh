#/bin/bash
arm-linux-gnueabihf-gcc -I /home/xiaoke/tools/png/include -L /home/xiaoke/tools/png/lib -L /home/xiaoke/tools/zlib/lib -lpng -lz -o show_png show_png.c