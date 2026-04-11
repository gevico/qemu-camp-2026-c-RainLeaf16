#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    const char *old_start = cmd + 2;
    const char *old_end = strchr(old_start, '/');
    const char *new_end;

    if (old_end == NULL) {
        return -1;
    }

    new_end = strchr(old_end + 1, '/');
    if (new_end == NULL || *(new_end + 1) != '\0') {
        return -1;
    }

    size_t old_len = (size_t)(old_end - old_start);
    size_t new_len = (size_t)(new_end - (old_end + 1));

    *old_str = malloc(old_len + 1);
    *new_str = malloc(new_len + 1);
    if (*old_str == NULL || *new_str == NULL) {
        free(*old_str);
        free(*new_str);
        return -1;
    }

    memcpy(*old_str, old_start, old_len);
    (*old_str)[old_len] = '\0';

    memcpy(*new_str, old_end + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    char *pos = strstr(str, old);
    char buffer[MAX_LINE_LENGTH];
    size_t prefix_len;

    if (pos == NULL) {
        return;
    }

    prefix_len = (size_t)(pos - str);
    snprintf(buffer, sizeof(buffer), "%.*s%s%s",
             (int)prefix_len, str, new, pos + strlen(old));
    strncpy(str, buffer, MAX_LINE_LENGTH - 1);
    str[MAX_LINE_LENGTH - 1] = '\0';
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;
    
    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    
    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
