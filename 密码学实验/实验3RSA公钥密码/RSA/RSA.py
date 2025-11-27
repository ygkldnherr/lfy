import random


def quick_power(base, exp, mod):
    result = 1
    base = base % mod
    while exp:
        if exp & 1:
            result = (result * base) % mod
        base = (base * base) % mod  # 这里原代码漏了赋值
        exp >>= 1
    return result



def is_probable_prime(n, trials=5):
    assert n >= 2
    # 2是素数~
    if n == 2:
        return True
    # 先判断n是否为偶数，用n&1==0的话效率更高
    if n % 2 == 0:
        return False
    # 把n-1写成(2^s)*d的形式
    s = 0
    d = n - 1
    while True:
        quotient, remainder = divmod(d, 2)
        if remainder == 1:
            break
        s += 1
        d = quotient
    assert (2 ** s * d == n - 1)

    # 测试以a为底时，n是否为合数
    def try_composite(a):
        if quick_power(a, d, n) == 1:  # 相当于(a^d)%n
            return False
        for i in range(s):
            if quick_power(a, 2 ** i * d, n) == n - 1:  # 相当于(a^((2^i)*d))%n
                return False
        return True  # 以上条件都不满足时，n一定是合数

    # trials为测试次数，默认测试5次
    # 每次的底a是不一样的，只要有一次未通过测试，则判定为合数
    for i in range(trials):
        a = random.randrange(2, n)
        if try_composite(a):
            return False

    return True  # 通过所有测试，n很大可能为素数


def generate_large_prime(n):
    # 生成一个 n 位的随机数，最高位不能为 0
    p = random.randint(10 ** (n - 1), 10 ** n - 1)

    # 确保 p 是奇数
    if p % 2 == 0:
        p += 1

    # 10000 以下的素数（共 1228 个），用于快速筛选
    small_primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]  # 这里只是部分，完整列表需预计算

    while True:
        # 先排除能被小素数整除的情况
        if any(p % prime == 0 for prime in small_primes if p != prime):
            p += 2
            continue

        # 进行 Miller-Rabin 素性测试
        if is_probable_prime(p):
            return p

        # 若不是素数，继续尝试下一个奇数
        p += 2


def gcd(a, b):
    while b:
        a, b = b, a % b
    return a


def mod_inverse(e, phi):
    # 计算 e 在模 phi 下的逆元
    def extended_gcd(a, b):
        if a == 0:
            return b, 0, 1
        gcd, x1, y1 = extended_gcd(b % a, a)
        x = y1 - (b // a) * x1
        y = x1
        return gcd, x, y

    gcd, x, _ = extended_gcd(e, phi)
    if gcd != 1:
        raise ValueError("e has no modular inverse")
    return x % phi


class RSA:
    def __init__(self):
        self.p = generate_large_prime(100)
        self.q = generate_large_prime(100)
        self.n = self.p * self.q
        self.fai_n = (self.p - 1) * (self.q - 1)

        # 选择公钥 e，通常选择 65537 作为 e
        self.e = 65537
        if gcd(self.e, self.fai_n) != 1:
            raise ValueError("e is not coprime with φ(n)")

        # 计算私钥 d
        self.d = mod_inverse(self.e, self.fai_n)

    def encrypt(self, plaintext):
        plaintext_int = int.from_bytes(plaintext.encode(), 'big')
        return quick_power(plaintext_int, self.e, self.n)

    def decrypt(self, ciphertext):
        decrypted_int = quick_power(ciphertext, self.d, self.n)
        return decrypted_int.to_bytes((decrypted_int.bit_length() + 7) // 8, 'big').decode()

def main():
    rsa = RSA()
    print(f"公钥为(e, n):({rsa.e, rsa.n})")
    print(f"私钥为(d, n):({rsa.d, rsa.n})")
    plaintext = input("请输入明文：")
    ciphertext = rsa.encrypt(plaintext)
    print(f"密文为：{ciphertext}")
    print(f"解密后的明文为：{rsa.decrypt(ciphertext)}")

if __name__ == '__main__':
    main()