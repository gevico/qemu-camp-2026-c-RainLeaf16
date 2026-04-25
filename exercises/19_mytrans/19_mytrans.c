// main.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int is_word_char(unsigned char c) {
  return isalpha(c) || c == '\'';
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int main() {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen("text.txt", "r");
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

    char *word = strtok(line, " \t");
    while (word != NULL) {
      char normalized[256];
      int pos = 0;

      // 查词前先做“归一化”：去掉标点，只保留字母/单引号并统一转小写。
      for (int i = 0; word[i] != '\0' && pos < (int)sizeof(normalized) - 1; i++) {
        unsigned char c = (unsigned char)word[i];
        if (is_word_char(c)) {
          normalized[pos++] = (char)tolower(c);
        }
      }
      normalized[pos] = '\0';

      if (pos > 0) {
        const char *translation = hash_table_lookup(table, normalized);
        printf("原文: %s\t", normalized);
        if (translation != NULL) {
          printf("翻译: %s\n", translation);
        } else {
          printf("未找到该单词的翻译。\n");
        }
      }

      word = strtok(NULL, " \t");
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}
