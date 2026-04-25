#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#if DEBUG_LEVEL >= 3
#define DEBUG_PRINT(fmt, ...)                                                        \
    do {                                                                             \
        void *frames[16];                                                            \
        int n = backtrace(frames, 16);                                               \
        printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); \
        backtrace_symbols_fd(frames, n, fileno(stdout));                             \
    } while (0)
#elif DEBUG_LEVEL == 2
#define DEBUG_PRINT(fmt, ...) \
    do { printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); } while (0)
#elif DEBUG_LEVEL == 1
#define DEBUG_PRINT(fmt, ...) \
    do { printf("DEBUG: func=%s, line=%d\n", __func__, __LINE__); } while (0)
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#endif



























//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
#line 48
    DEBUG_PRINT("x=%d", x);
#line 50
}

int main() {
    test();
    return 0;
}
