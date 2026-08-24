// gcc ent-rop-y.c -w -no-pie -fno-stack-protector -Wl,-z,relro,-z,now -o ent-rop-y
#include <stdio.h>

void setup_io() {
    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int main() {
    setup_io();
    if (system("echo It’s dangerous to go alone! Take this: /bin/sh"))
        __asm__("add $0xFF01FF01, %edi; ret");
    
    char buf[16];
    read(0, buf, 1337);
}
