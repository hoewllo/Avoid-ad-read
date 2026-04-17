#!/bin/bash
# 从VERSION文件读取版本号的脚本（简单格式：只包含版本号）
# 适用于Linux/macOS和GitHub Actions

VERSION_FILE="../VERSION"

# 检查文件是否存在
if [ ! -f "$VERSION_FILE" ]; then
    echo "Error: $VERSION_FILE not found" >&2
    exit 1
fi

# 读取版本号（简单格式：只包含版本号，如 1.0.5）
VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')
echo "$VERSION"

# 根据参数输出不同的信息
case "$1" in
    "major")
        VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')
        IFS='.' read -r MAJOR MINOR PATCH <<< "$VERSION"
        echo "$MAJOR"
        ;;
    "minor")
        VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')
        IFS='.' read -r MAJOR MINOR PATCH <<< "$VERSION"
        echo "$MINOR"
        ;;
    "patch")
        VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')
        IFS='.' read -r MAJOR MINOR PATCH <<< "$VERSION"
        echo "$PATCH"
        ;;
    "full")
        VERSION=$(head -n 1 "$VERSION_FILE" | tr -d '[:space:]')
        echo "$VERSION"
        ;;
    "name")
        echo "epub_cleaner"
        ;;
    "all"|"")
        read_version
        ;;
    *)
        echo "Usage: $0 [major|minor|patch|full|name|all]" >&2
        echo "  major    - 输出主版本号" >&2
        echo "  minor    - 输出次版本号" >&2
        echo "  patch    - 输出修订版本号" >&2
        echo "  full     - 输出完整版本号" >&2
        echo "  name     - 输出项目名称" >&2
        echo "  all      - 输出所有版本信息" >&2
        exit 1
        ;;
esac