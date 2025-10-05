# 调试版本 - 显示所有信息
CC := gcc
CFLAGS := -Iinclude -Wall -Wextra -g -lm
# -Iinclude: 指定头文件搜索路径
# -Wall: 开启所有警告
# -Wextra: 额外警告  
# -g: 生成调试信息
# -lm: 链接数学库
SRC_DIR := function
BUILD_DIR := build

# 显示调试信息
$(info === 开始调试 ===)
$(info 当前目录: $(shell pwd))
$(info SRC_DIR: $(SRC_DIR))
$(info 查找模式: $(wildcard $(SRC_DIR)/*.c))

SOURCES := $(wildcard $(SRC_DIR)/*.c)
	
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# 语法: $(var:pattern=replacement)
# 把 SOURCES 中的 "function/xxx.c" 替换成 "build/xxx.o"
$(info SOURCES找到的文件: $(SOURCES))
$(info OBJECTS: $(OBJECTS))
$(info === 调试结束 ===)

TARGET := $(BUILD_DIR)/app

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean