import sys

def extract_unique_strings(file_path):
    unique_strings = set()  # 使用集合存储唯一字符串

    with open(file_path, 'r') as file:
        for line in file:
            # 找到::的位置
            if '::' in line:
                part = line.split('::')[0].strip()  # 提取::之前的部分并去掉空格
                
                # 检查是否只包含字母和数字
                if part.isalnum():
                    unique_strings.add(part)  # 添加到集合中

    # 打印所有唯一的字符串
    for unique_string in unique_strings:
        print(unique_string)

if __name__ == "__main__":
    # 检查是否提供了命令行参数
    if len(sys.argv) < 2:
        print("Usage: python extract_unique.py <file_path>")
        sys.exit(1)

    file_path = sys.argv[1]  # 获取命令行参数中的文件路径
    extract_unique_strings(file_path)
