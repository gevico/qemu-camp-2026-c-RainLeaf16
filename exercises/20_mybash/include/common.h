#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COMMON_PATH_MAX 4096

#define SYSCALL_CMD0(name) \
    extern int __cmd_##name(void);

#define SYSCALL_CMD1(name, arg1) \
    extern int __cmd_##name(arg1);

#define SYSCALL_CMD2(name, arg1, arg2) \
    extern int __cmd_##name(arg1, arg2);

SYSCALL_CMD1(mytrans, const char*);
SYSCALL_CMD1(myfile, const char*);
SYSCALL_CMD2(mysed, const char*, const char*);
SYSCALL_CMD1(mywc, const char*);

static inline void get_project_root(char *output, size_t size) {
    char cwd[COMMON_PATH_MAX];
    char *marker;

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        snprintf(output, size, ".");
        return;
    }

    marker = strstr(cwd, "/exercises/20_mybash");
    if (marker != NULL) {
        marker += strlen("/exercises/20_mybash");
        *marker = '\0';
        snprintf(output, size, "%s", cwd);
        return;
    }

    if (strlen(cwd) >= 6 && strcmp(cwd + strlen(cwd) - 6, "/tests") == 0) {
        snprintf(output, size, "%s/../exercises/20_mybash", cwd);
        return;
    }

    snprintf(output, size, "%s", cwd);
}

static inline void resolve_workspace_path(const char *input, char *output, size_t size) {
    const char *prefix = "/workspace/exercises/20_mybash";
    size_t prefix_len = strlen(prefix);

    if (input == NULL) {
        output[0] = '\0';
        return;
    }

    if (strncmp(input, prefix, prefix_len) == 0) {
        char root[COMMON_PATH_MAX];

        get_project_root(root, sizeof(root));
        snprintf(output, size, "%s%s", root, input + prefix_len);
        return;
    }

    snprintf(output, size, "%s", input);
}

#endif //! COMMON_H
