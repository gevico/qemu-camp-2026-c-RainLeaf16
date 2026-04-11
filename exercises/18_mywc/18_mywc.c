#include "mywc.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;
  const char* filepath = "./text.txt";

  process_file(filepath);
  return 0;
}
