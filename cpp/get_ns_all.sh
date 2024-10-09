#!/bin/bash

mkdir -p ns

# 遍历指定目录
for dir in ~/release-1.00.13/*; do
    if [[ -d "$dir" ]]; then  # 确保是目录
        FILENAME=$(basename "$dir")  # 获取目录名称
        
        # 构造二进制文件路径
        binary_file="$dir/bin/$FILENAME"
        
        # 检查二进制文件是否存在且可执行
        if [[ -x "$binary_file" ]]; then
            # 生成符号文件
            ./get_symbols "$binary_file" > "${FILENAME}.txt"
            
            # 生成命名空间文件
            python get_namespace.py "${FILENAME}.txt" > "ns/${FILENAME}_ns.txt"
            
            echo "Processed $FILENAME"
        else
            echo "$binary_file does not exist or is not executable."
        fi
    else
        echo "$dir is not a directory."
    fi
done
