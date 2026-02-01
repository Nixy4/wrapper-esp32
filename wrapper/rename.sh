#!/bin/bash

# 列出所有要重命名的文件对
files=(
    "naudio.cpp:waudio.cpp"
    "naudio.hpp:waudio.hpp"
    "ndisplay.cpp:wdisplay.cpp"
    "ndisplay.hpp:wdisplay.hpp"
    "ni2c.cpp:wi2c.cpp"
    "ni2c.hpp:wi2c.hpp"
    "ni2s.cpp:wi2s.cpp"
    "ni2s.hpp:wi2s.hpp"
    "nlogger.cpp:wlogger.cpp"
    "nlogger.hpp:wlogger.hpp"
    "nlvgl.cpp:wlvgl.cpp"
    "nlvgl.hpp:wlvgl.hpp"
    "nspi.cpp:wspi.cpp"
    "nspi.hpp:wspi.hpp"
    "ntouch.cpp:wtouch.cpp"
    "ntouch.hpp:wtouch.hpp"
)

# 第一步：创建新文件并更新内容
for pair in "${files[@]}"; do
    old_name="${pair%:*}"
    new_name="${pair#*:}"
    
    if [ -f "$old_name" ]; then
        echo "处理: $old_name -> $new_name"
        
        # 读取旧文件，替换所有 nix:: -> wrapper::，n开头的标识符 -> w开头
        sed -e 's/nix::/wrapper::/g' \
            -e 's/namespace nix/namespace wrapper/g' \
            -e 's/_nix/_wrapper/g' \
            -e 's/\"nix\"/\"wrapper\"/g' \
            "$old_name" > "$new_name"
        
        echo "  ✓ 创建 $new_name"
    fi
done

# 第二步：更新新创建的文件中的头文件引用
for pair in "${files[@]}"; do
    old_name="${pair%:*}"
    new_name="${pair#*:}"
    
    if [ -f "$new_name" ]; then
        # 替换所有旧的include
        sed -i "s/#include \"n/#include \"w/g" "$new_name"
        sed -i "s/#include <n/#include <w/g" "$new_name"
        echo "  ✓ 更新 $new_name 中的头文件引用"
    fi
done

# 第三步：删除旧文件
for pair in "${files[@]}"; do
    old_name="${pair%:*}"
    new_name="${pair#*:}"
    
    if [ -f "$old_name" ]; then
        rm "$old_name"
        echo "  ✓ 删除 $old_name"
    fi
done

echo "完成！"
