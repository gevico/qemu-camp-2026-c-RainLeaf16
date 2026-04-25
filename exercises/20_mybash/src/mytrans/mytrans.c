// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
    char *start = str;
    size_t len;

    // 先去掉行首空白。
    while (isspace((unsigned char)*start)) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }

    len = strlen(str);
    // 再去掉行尾空白，方便按固定格式解析词典。
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\r\n")] = '\0';
    trim(line);

    if (line[0] == '\0') {
      continue;
    }

    if (line[0] == '#') {
      // 遇到新词条标题前，先把上一条完整词条写入哈希表。
      if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        if (!hash_table_insert(table, current_word, current_translation)) {
          fclose(file);
          return -1;
        }
        if (dict_count != NULL) {
          (*dict_count)++;
        }
      }

      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      trim(current_word);
      current_translation[0] = '\0';
      in_entry = 1;
      continue;
    }

    if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
      const char *translation = line + 6;
      while (isspace((unsigned char)*translation)) {
        translation++;
      }

      if (current_translation[0] != '\0') {
        strncat(current_translation, " ",
                sizeof(current_translation) - strlen(current_translation) - 1);
      }
      strncat(current_translation, translation,
              sizeof(current_translation) - strlen(current_translation) - 1);
    }
  }

  if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
    if (!hash_table_insert(table, current_word, current_translation)) {
      fclose(file);
      return -1;
    }
    if (dict_count != NULL) {
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  char dict_path[COMMON_PATH_MAX];
  char input_path[COMMON_PATH_MAX];
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  resolve_workspace_path("/workspace/exercises/20_mybash/src/mytrans/dict.txt",
                         dict_path, sizeof(dict_path));
  if (load_dictionary(dict_path, table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  resolve_workspace_path(filename, input_path, sizeof(input_path));
  FILE* file = fopen(input_path, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
        continue;
    }

    char *word = strtok(line, " ");
    while (word != NULL) {
      char normalized[256];
      int pos = 0;

      // 查词前统一转成“小写纯单词”，避免标点影响查找。
      for (int i = 0; word[i] != '\0' && pos < (int)sizeof(normalized) - 1; i++) {
        unsigned char c = (unsigned char)word[i];
        if (isalpha(c) || c == '\'') {
          normalized[pos++] = (char)tolower(c);
        }
      }
      normalized[pos] = '\0';

      if (pos > 0) {
        const char *translation = hash_table_lookup(table, normalized);
        printf("原文: %s\t", normalized);
        if (translation) {
          printf("翻译: %s\n", translation);
        } else {
          printf("未找到该单词的翻译。\n");
        }
      }

      word = strtok(NULL, " ");
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}
