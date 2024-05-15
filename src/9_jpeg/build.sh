#/bin/bash
arm-linux-gnueabihf-gcc -I /home/xiaoke/tools/jpeg/include -L /home/xiaoke/tools/jpeg/lib -ljpeg -o show_jpeg show_jpeg.c