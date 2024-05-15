#/bin/bash
arm-linux-gnueabihf-gcc -I /home/xiaoke/tools/tslib/include -L /home/xiaoke/tools/tslib/lib -lts -o ts_read_mt ts_read_mt.c