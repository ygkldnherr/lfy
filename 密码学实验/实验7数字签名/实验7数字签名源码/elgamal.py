import gmpy2
import os
import random
import math
import hashlib
from typing import Tuple, List, Union


def get_large_prime_length(n: int) -> int:
    """生成一个n位的随机大素数"""
    p = gmpy2.next_prime(gmpy2.mpz(2) ** (n - 1))
    return int(p)


def fastExpMod(m: int, e: int, n: int) -> int:
    """
    快速幂取模法 / 模的重复平方法
    m^e mod n
    :param m: 底数
    :param e: 幂数
    :param n: 模数
    :return: 模的结果
    """
    result = 1
    while e != 0:
        if (e & 1) == 1:
            result = (result * m) % n
        e >>= 1
        m = (m * m) % n
    return result


def list_split(items: str, n: int) -> List[str]:
    """将字符串按指定长度分割"""
    return [items[i:i + n] for i in range(0, len(items), n)]


def makefile(path: str, content: List[int]) -> None:
    """
    以二进制编码保存文件
    :param path: 保存地址
    :param content: 二进制数据列表
    """
    try:
        with open(path, 'wb') as f:
            for i in content:
                f.write(int(i).to_bytes(1, 'little'))
        print(f'文件已生成: {path}')
    except Exception as e:
        print(f'错误：{e}')
        exit(1)


def get_data(path: str) -> bytes:
    """
    读取文件的二进制数据
    :param path: 文件地址
    :return: 二进制数据
    """
    with open(path, 'rb') as f:
        return f.read()

def standard_data(data_all: bytes, length: int) -> List[str]:
    data_list = ''
    for data in data_all:
        if data == 0:
            long = 1
        else:
            long = int(math.log10(data)) + 1
        data_list += str(long * 10 ** 3 + data)

    # ✅ 强制确保 length >= 4
    if length < 4:
        length = 4
    else:
        length = max(4, int(length / 4 - 1) * 4)

    return list_split(data_list, length)


def recovery_data(res: str) -> List[int]:
    """恢复原始数据"""
    res = list_split(res, 4)
    m = []
    for data in res:
        length = int(int(data) / 1000)
        m.append(int(data) % 10 ** length)
    return m


def save_ctext(path: str, data_list: List[int], max_length: int) -> None:
    """
    保存密文文件，规范数据格式
    :param path: 文件路径
    :param data_list: 数据列表
    :param max_length: 最大长度
    """
    m_s = ''
    for data in data_list:
        if len(str(data)) < max_length:
            m_s += '0' * (max_length - len(str(data))) + str(data)
        else:
            m_s += str(data)
    m_s = list_split(m_s, 2)
    makefile(path, [int(x) for x in m_s])


def get_key(n: int = 20) -> Tuple[int, int, int, int]:
    """
    生成ElGamal密钥对
    :param n: 素数位数
    :return: (p, g, y, x) 公钥和私钥
    """
    p = get_large_prime_length(n)
    yn = p - 1
    f = 2
    x_list = []

    # 分解p-1的质因数
    while yn != 1:
        if gmpy2.is_prime(int(yn)):
            x_list.append(int(yn))
            break
        if gmpy2.is_prime(f):
            if yn % f == 0:
                x_list.append(f)
                while yn % f == 0:
                    yn = yn // f
            if yn == 1:
                break
        f += 1

    # 寻找生成元g
    yn = p - 1
    error_list = []
    while True:
        i = random.randint(2, p)
        if i in error_list:
            continue
        error_list.append(i)

        k = 0
        for j in x_list:
            if fastExpMod(i, int(yn // j), p) == 1:
                break
            k += 1

        if k == len(x_list):
            x = random.randint(1, p - 1)
            y = fastExpMod(i, x, p)
            return p, i, y, x


def exgcd(a: int, b: int, x: int = 1, y: int = 0) -> Tuple[int, int, int]:
    """扩展欧几里得算法"""
    if b == 0:
        return a, x, y
    gcd, x, y = exgcd(b, a % b, x, y)
    return gcd, y, x - a // b * y


def cal_inv(a: int, m: int) -> int:
    """
    计算模逆元
    :param a: 数字
    :param m: 模数
    :return: 逆元，若无则返回-1
    """
    gcd, x, y = exgcd(a, m)
    return (x % m + m) % m if gcd == 1 else -1


def encrypt(m: int, p: int, g: int, y: int) -> Tuple[int, int]:
    """
    ElGamal加密
    :param m: 明文
    :param p: 公钥参数
    :param g: 公钥参数
    :param y: 公钥
    :return: (c1, c2) 密文对
    """
    while True:
        k = random.randint(1, p - 1)
        if exgcd(k, p - 1)[0] == 1:
            break
    c1 = fastExpMod(g, k, p)
    c = fastExpMod(y, k, p)
    c2 = m * c % p
    return c1, c2


def decrypt(c1: int, c2: int, x: int, p: int) -> int:
    """
    ElGamal解密
    :param c1: 密文第一部分
    :param c2: 密文第二部分
    :param x: 私钥
    :param p: 公钥参数
    :return: 解密后的明文
    """
    c1_x = fastExpMod(c1, x, p)
    inv_c1 = cal_inv(c1_x, p)
    return c2 * inv_c1 % p


def data_encrypt(p: int, g: int, y: int, c_path: str, path: str = None, m_data: str = None) -> Tuple[
    str, str, int, int]:
    """
    加密数据
    :param p: 公钥参数
    :param g: 公钥参数
    :param y: 公钥
    :param c_path: 密文保存路径
    :param path: 待加密文件路径
    :param m_data: 待加密字符串
    :return: (c1_path, c2_path, c1_max_len, c2_max_len)
    """
    data = get_data(path) if path else m_data.encode()
    data_list = standard_data(data, length=int(math.log10(p)) + 1)

    c1_l, c2_l = [], []
    c1_max_len, c2_max_len = 0, 0

    for data in data_list:
        num = int(data)
        c1, c2 = encrypt(num, p, g, y)
        c1_l.append(c1)
        c2_l.append(c2)

        len1 = 1 if c1 == 0 else int(math.log10(c1)) + 1
        len2 = 1 if c2 == 0 else int(math.log10(c2)) + 1

        c1_max_len = max(c1_max_len, len1)
        c2_max_len = max(c2_max_len, len2)

    os.makedirs(c_path, exist_ok=True)
    path1 = os.path.join(c_path, 'c1.txt')
    path2 = os.path.join(c_path, 'c2.txt')

    save_ctext(path1, c1_l, c1_max_len)
    save_ctext(path2, c2_l, c2_max_len)

    return path1, path2, c1_max_len, c2_max_len


def data_decrypt(p: int, x: int, path1: str, path2: str, c1_max_len: int, c2_max_len: int, path_m: str = None) -> Union[
    str, None]:
    """
    解密数据
    :param p: 公钥参数
    :param x: 私钥
    :param path1: c1文件路径
    :param path2: c2文件路径
    :param c1_max_len: c1最大长度
    :param c2_max_len: c2最大长度
    :param path_m: 解密文件保存路径
    :return: 若path_m为None则返回解密字符串，否则返回None
    """

    def process_file(path: str, max_len: int) -> List[str]:
        data = get_data(path)
        m_list = ''
        for i, m in enumerate(data):
            if i == len(data) - 1:
                m_list += str(m)
            elif m == 0:
                m_list += '00'
            else:
                m_list += '0' * (1 - int(math.log10(m))) + str(m)
        return list_split(m_list, max_len)

    c1 = process_file(path1, c1_max_len)
    c2 = process_file(path2, c2_max_len)

    if len(c1) != len(c2):
        print('密文有错误')
        exit(1)

    res = ''
    for i in range(len(c1)):
        res += str(decrypt(int(c1[i]), int(c2[i]), x, p))

    m = recovery_data(res)

    if path_m:
        makefile(path_m, m)
        return None
    else:
        return bytes(m).decode(errors='ignore')


def main():
    print("ElGamal加密系统")
    print("1. 生成密钥对")
    print("2. 加密文件")
    print("3. 解密文件")
    print("4. 加密字符串")
    print("5. 解密字符串")
    choice = input("请选择操作(1-5): ")

    if choice == '1':
        n = int(input("请输入素数位数(推荐20以上): "))
        p, g, y, x = get_key(n)
        print(f"公钥(p,g,y):\n{p}\n{g}\n{y}")
        print(f"私钥x: {x}")
        with open('key.pub', 'w') as f:
            f.write(f"{p}\n{g}\n{y}")
        with open('key.priv', 'w') as f:
            f.write(f"{x}")
        print("密钥已保存到key.pub和key.priv")

    elif choice in ['2', '4']:
        if not os.path.exists('key.pub'):
            print("请先生成密钥对!")
            return

        with open('key.pub') as f:
            p, g, y = map(int, f.read().splitlines())

        if choice == '2':
            path = input("请输入要加密的文件路径: ")
            c_path = input("请输入密文保存目录: ")
            data_encrypt(p, g, y, c_path, path=path)
            print("加密完成!")
        else:
            text = input("请输入要加密的字符串: ")
            c_path = input("请输入密文保存目录: ")
            path1, path2, c1_max_len, c2_max_len = data_encrypt(p, g, y, c_path, m_data=text)
            # 打印提示
            print(f"加密完成！c1_max_len = {c1_max_len}, c2_max_len = {c2_max_len}")

    elif choice in ['3', '5']:
        if not (os.path.exists('key.pub') and os.path.exists('key.priv')):
            print("请先生成密钥对!")
            return

        with open('key.pub') as f:
            p, g, y = map(int, f.read().splitlines())
        with open('key.priv') as f:
            x = int(f.read())

        if choice == '3':
            path1 = input("请输入c1.txt路径: ")
            path2 = input("请输入c2.txt路径: ")
            c1_len = int(input("请输入c1最大长度: "))
            c2_len = int(input("请输入c2最大长度: "))
            out_path = input("请输入解密文件保存路径: ")
            data_decrypt(p, x, path1, path2, c1_len, c2_len, out_path)
            print("解密完成!")
        else:
            path1 = input("请输入c1.txt路径: ")
            path2 = input("请输入c2.txt路径: ")
            c1_len = int(input("请输入c1最大长度: "))
            c2_len = int(input("请输入c2最大长度: "))
            result = data_decrypt(p, x, path1, path2, c1_len, c2_len)
            print("解密结果:", result)

    else:
        print("无效选择!")


if __name__ == '__main__':
    main()