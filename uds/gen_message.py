import random
import string


def generate_random_strings(m, n):
    """
    生成 n 条长度为 m 的随机字符串，字符串内容只包括大小写字母。

    :param m: 每个字符串的长度
    :param n: 总共生成的字符串数量
    :return: 包含 n 条随机字符串的列表
    """
    random_strings = []

    # 字符集只包括大小写字母
    letters = string.ascii_letters  # 包含大写字母和小写字母

    for _ in range(n):
        random_string = "".join(random.choice(letters) for _ in range(m))
        random_strings.append(random_string)

    return random_strings


if __name__ == "__main__":
    m = 300  # 每个字符串的长度
    n = 1000000  # 生成的字符串数量

    random_strings = generate_random_strings(m, n)

    # 打印生成的随机字符串
    for idx, s in enumerate(random_strings, 1):
        print(f"{s}")
