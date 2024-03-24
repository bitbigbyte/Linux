# uboot移植
## 1.uboot移植的一般流程:
1. 在uboot中找到参考的开发平台，一般是原厂的开发板。
2. 参考原厂开发板移植uboot到我们所使用的开发板上。
> eg:正点原子的I.MX6ULL开发板参考的是NXP官方的I.MX6ULL EVK开发板做的硬件，因此我们在移植uboot的时候就可以以NXP官方的I.MX6ULL EVK开发板为蓝本。
## 2.在uboot中添加自己的开发板
1. I.MX6UL/6ULL 有 9x9mm 和 14x14mm 两种尺寸的，所
以我们可以看到会有mx6ull_9x9和mx6ull_14x14开头的默认配置文件。我们使用的是14x14mm
的芯片，所以关注 mx6ull_14x14 开头的默认配置文件。正点原子的 I.MX6ULL 有 EMMC 和
NAND 两 个 版 本 的 ， 因 此 我 们 最 终 只 需 要 关 注 mx6ull_14x14_evk_emmc_defconfig 和
mx6ull_14x14_evk_nand_defconfig 这两个配置文件就行了。本章我们讲解 EMMC 版本的移植
（NAND 版本移植很多类似），所以使用 mx6ull_14x14_evk_emmc_defconfig 作为默认配置文件。
