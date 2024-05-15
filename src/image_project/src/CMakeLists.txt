# 设置 C 交叉编译器  
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")  

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 定义可执行文件目标
add_executable(image_player main.c show_bmp.c show_jpeg.c show_png.c)

# 头文件搜索路径
target_include_directories(image_player PRIVATE /home/xiaoke/tools/tslib/include /home/xiaoke/tools/jpeg/include /home/xiaoke/tools/png/include)
# 库文件搜索路径
target_link_directories(image_player PRIVATE /home/xiaoke/tools/tslib/lib /home/xiaoke/tools/jpeg/lib /home/xiaoke/tools/png/lib /home/xiaoke/tools/zlib/lib)	
# 链接库
target_link_libraries(image_player PRIVATE ts jpeg png z)	