"""
（1）编写程序实现D-H密钥交换协议；
（2）编写程序实现ElGamal公钥密码算法；
（3）体会D-H密钥交换到ElGamal公钥密码算法转换的方法。
"""
import random

def is_prime(n):
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0:
        return False
    for i in range(3, int(n**0.5)+1, 2):
        if n % i == 0:
            return False
    return True

def find_primitive_root(p):
    if p == 2:
        return 1
    phi = p - 1
    factors = set()
    n = phi
    i = 2
    while i * i <= n:
        if n % i == 0:
            factors.add(i)
            while n % i == 0:
                n //= i
        i += 1
    if n > 1:
        factors.add(n)

    for g in range(2, p):
        flag = True
        for factor in factors:
            if pow(g, phi // factor, p) == 1:
                flag = False
                break
        if flag:
            return g
    return -1  # 没有原根（理论上不会发生）

def dh_key_exchange():
    while True:
        try:
            p = int(input("请输入一个素数 p："))
            if is_prime(p):
                break
            else:
                print("输入的不是素数，请重新输入。")
        except ValueError:
            print("输入无效，请输入整数。")

    g = find_primitive_root(p)
    if g == -1:
        print("找不到原根")
        return

    a = random.randint(2, p - 2)
    b = random.randint(2, p - 2)

    A = pow(g, a, p)
    B = pow(g, b, p)

    shared_key_a = pow(B, a, p)
    shared_key_b = pow(A, b, p)
    print("-------------------------")
    print(f"选取素数 p = {p}")
    print(f"找到原根 g = {g}")
    print(f"A 的私钥 a = {a}")
    print(f"B 的私钥 b = {b}")
    print(f"A 的公钥 A = g^a mod p = {A}")
    print(f"B 的公钥 B = g^b mod p = {B}")
    print(f"A 计算共享密钥: (B^a) mod p = {shared_key_a}")
    print(f"B 计算共享密钥: (A^b) mod p = {shared_key_b}")
    print("共享密钥一致！" if shared_key_a == shared_key_b else "共享密钥不一致！")

if __name__ == "__main__":
    dh_key_exchange()
