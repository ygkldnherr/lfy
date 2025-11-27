import random
from dh import is_prime, find_primitive_root  # 复用 DH 中的函数

# 扩展欧几里得算法
def extended_gcd(a, b):
    if b == 0:
        return a, 1, 0
    g, y, x = extended_gcd(b, a % b)
    return g, x, y - (a // b) * x

# 求模逆元
def modinv(a, m):
    g, x, _ = extended_gcd(a, m)
    if g != 1:
        raise Exception("No modular inverse")
    return x % m

def elgamal_keygen():
    while True:
        try:
            p = int(input("请输入一个素数 p："))
            if is_prime(p):
                break
            else:
                print("输入的不是素数，请重新输入。")
        except ValueError:
            print("请输入一个有效整数。")

    g = find_primitive_root(p)
    if g == -1:
        print("找不到原根，程序终止。")
        return None

    x = random.randint(2, p - 2)  # 私钥
    y = pow(g, x, p)              # 公钥部分

    print(f"\n公钥: (p = {p}, g = {g}, y = g^x mod p = {y})")
    print(f"私钥: x = {x}")

    return p, g, y, x

def elgamal_encrypt(p, g, y, m):
    k = random.randint(2, p - 2)
    c1 = pow(g, k, p)
    s = pow(y, k, p)
    c2 = (m * s) % p
    return (c1, c2)

def elgamal_decrypt(p, x, c1, c2):
    s = pow(c1, x, p)
    s_inv = modinv(s, p)
    m = (c2 * s_inv) % p
    return m

def main():
    # 密钥生成
    p, g, y, x = elgamal_keygen()

    # 用户输入明文
    m = int(input("\n请输入要加密的明文（整数 < p）："))
    if not (0 < m < p):
        print("明文不合法，应满足 0 < m < p")
        return

    # 加密
    c1, c2 = elgamal_encrypt(p, g, y, m)
    print(f"\n加密结果：密文对 (c1 = {c1}, c2 = {c2})")

    # 解密
    decrypted = elgamal_decrypt(p, x, c1, c2)
    print(f"解密得到明文：{decrypted}")
    print("解密成功！" if decrypted == m else "解密失败！")

if __name__ == "__main__":
    main()
