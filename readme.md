运行在ESP32上的玩具操作系统

功能：

- [x] 内存管理（貌似直接new delete就可以，getFreeHeap监视一下）
- [x] 进程调度（RR，程序层为lua）
- [ ] 文件管理（考虑加SD卡读写）
- [ ] 图形界面（使用TFT）[API完成，还有一些问题]
- [ ] 简单IO（GPIO轮询键盘输入，鼠标等）
- [ ] 多线程（emmm...)

文件结构：

- app  lua脚本文件，以及lua编译器，运行buildall.bat可以编译到data文件夹
- data 编译后的二进制lua文件，可以被操作系统执行
- l*.* 大部分l开头的文件都是lua的源码，部分经过魔改用来支持操作系统进程调度
- 其余文件为操作系统源码

编译环境：

Arduino ESP32开发板
TFT_eSPI库
SPI库
