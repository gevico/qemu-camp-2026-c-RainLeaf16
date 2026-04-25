#include <stdio.h>

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;
    
    __asm__ volatile (
        "movl %[in_a], %%eax\n\t"
        "movl %[in_b], %%ecx\n\t"
        "1:\n\t"
        "testl %%ecx, %%ecx\n\t"
        "je 2f\n\t"
        "xorl %%edx, %%edx\n\t"
        "divl %%ecx\n\t"
        "movl %%ecx, %%eax\n\t"
        "movl %%edx, %%ecx\n\t"
        "jmp 1b\n\t"
        "2:\n\t"
        : "=&a" (result)
        : [in_a] "r" (a), [in_b] "r" (b)
        : "ecx", "edx", "cc"
    );
    
    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}
