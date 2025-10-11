# 调试版本 - 显示所有信息
CC := gcc
CFLAGS := -Iinclude -Ilibs/logdir -Wall -Wextra -g -lm
# -Iinclude -Ilibs/logdir: 添加两个头文件路径
# -Wall: 开启所有警告
# -Wextra: 额外警告  
# -g: 生成调试信息
# -lm: 链接数学库

SRC_DIR := function libs/logdir
BUILD_DIR := build
TARGET := $(BUILD_DIR)/app

# 查找所有源文件（.c文件）
SOURCES := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
# 查找所有目标文件（.o文件），并将它们放在 build 目录
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

$(info === 开始调试 ===)
$(info 当前目录: $(shell pwd))
$(info 查找的源文件目录: $(SRC_DIR))
$(info 查找模式: $(wildcard $(SRC_DIR)/*.c))
$(info SOURCES找到的文件: $(SOURCES))
$(info OBJECTS: $(OBJECTS))
$(info === 调试结束 ===)

# 最终目标
$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ $(CFLAGS)

# 生成目标文件的规则
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)  # 确保目标文件目录存在
	$(CC) $(CFLAGS) -c $< -o $@  # 编译源文件到目标文件

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
