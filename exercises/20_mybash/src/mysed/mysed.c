#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }
    
    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;
    
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char *old_start = cmd + 2;
    const char *old_end = strchr(old_start, '/');
    const char *new_end;
    size_t old_len;
    size_t new_len;

    if (old_end == NULL) {
        return -1;
    }

    new_end = strchr(old_end + 1, '/');
    if (new_end == NULL || *(new_end + 1) != '\0') {
        return -1;
    }

    old_len = (size_t)(old_end - old_start);
    new_len = (size_t)(new_end - (old_end + 1));

    // 这两个字符串要在函数返回后继续使用，所以动态分配更安全。
    *old_str = malloc(old_len + 1);
    *new_str = malloc(new_len + 1);
    if (*old_str == NULL || *new_str == NULL) {
        free(*old_str);
        free(*new_str);
        *old_str = NULL;
        *new_str = NULL;
        return -1;
    }

    memcpy(*old_str, old_start, old_len);
    (*old_str)[old_len] = '\0';

    memcpy(*new_str, old_end + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    char *pos = strstr(str, old);
    char buffer[1024];
    size_t prefix_len;

    if (pos == NULL) {
        return;
    }

    prefix_len = (size_t)(pos - str);
    // 重新拼接出“前缀 + 新串 + 后缀”，实现第一次匹配的替换。
    snprintf(buffer, sizeof(buffer), "%.*s%s%s",
             (int)prefix_len, str, new, pos + strlen(old));
    strncpy(str, buffer, 1023);
    str[1023] = '\0';
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}
