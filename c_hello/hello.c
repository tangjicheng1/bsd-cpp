__attribute__((section(".multiboot")))
const unsigned long multiboot_header[] = {
    0x1BADB002,          // magic number
    0x00,                // flags
    -(0x1BADB002 + 0x00) // checksum
};

void printString(const char *str) {
  volatile char *video = (volatile char *)0xB8000;
  while (*str) {
    *video++ = *str++; // 写字符
    *video++ = 0x07;   // 设置字符属性（颜色）
  }
}

void _start() {
  printString("Hello, World!");
  while (1)
    ; // 防止程序结束
}
