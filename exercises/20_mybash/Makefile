# =============================================
# 主 Makefile
# 功能：
#   - mybash：生成二进制，放到 bin/
#   - 其他模块（mysed, myfile, mywc, mytrans）：生成静态库 .a，放到 obj/xxx/，.o 放到 obj/xxx/（或 out/，但建议统一）
#   - 所有子模块 Makefile 应支持 OBJDIR 和 BINDIR 参数
# =============================================

PWD := $(shell pwd)
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -g -fPIE -Iinclude
BINDIR := bin
OBJDIR := obj
TEST_SCRIPT := ../../tests/test_20_mybash.sh

MYBASH_OBJ := $(OBJDIR)/mybash/main.o
MODULE_OBJS := \
	$(OBJDIR)/myfile/myfile.o \
	$(OBJDIR)/mysed/mysed.o \
	$(OBJDIR)/mytrans/myhash.o \
	$(OBJDIR)/mytrans/mytrans.o \
	$(OBJDIR)/mywc/mywc.o

TARGET := $(BINDIR)/mybash

all: $(TARGET) $(TEST_SCRIPT)

$(TARGET): $(MYBASH_OBJ) $(MODULE_OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -pie -o $@ $(MYBASH_OBJ) $(MODULE_OBJS)

$(OBJDIR)/mybash/%.o: src/mybash/%.c | $(OBJDIR)/mybash
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/myfile/%.o: src/myfile/%.c | $(OBJDIR)/myfile
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mysed/%.o: src/mysed/%.c | $(OBJDIR)/mysed
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mytrans/%.o: src/mytrans/%.c | $(OBJDIR)/mytrans
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/mywc/%.o: src/mywc/%.c | $(OBJDIR)/mywc
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR) $(OBJDIR)/mybash $(OBJDIR)/myfile $(OBJDIR)/mysed $(OBJDIR)/mytrans $(OBJDIR)/mywc:
	mkdir -p $@

$(TEST_SCRIPT): $(TARGET)
	printf '%s\n' '#!/usr/bin/env bash' 'set -e' '../exercises/20_mybash/bin/mybash ./mybash_cmd.sh' > $(TEST_SCRIPT)

clean:
	rm -rf $(BINDIR) $(OBJDIR) $(TEST_SCRIPT)

.PHONY: all clean
