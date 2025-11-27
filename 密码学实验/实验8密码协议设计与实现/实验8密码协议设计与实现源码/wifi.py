import hashlib
import random
import base64
from math import gcd
from typing import NamedTuple
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

# 定义DSS公钥和签名结构
class PubKey(NamedTuple):
    p: int
    q: int
    g: int
    y: int


class Signature(NamedTuple):
    M: str
    r: int
    s: int


# DSS 签名类
class DSS:
    def __init__(self):
        self.pub_key = PubKey(
            p=0xd411a4a0e393f6aab0f08b14d18458665b3e4dbdce2544543fe365cf71c8622412db6e7dd02bbe13d88c58d7263e90236af17ac8a9fe5f249cc81f427fc543f7,
            q=0xb20db0b101df0c6624fc1392ba55f77d577481e5,
            g=0xb3085510021f999049a9e7cd3872ce9958186b5007e7adaf25248b58a3dc4f71781d21f2df89b71747bd54b323bbecc443ec1d3e020dadabbf7822578255c104,
            y=0xb32fbec03175791df08c3f861c81df7de7e0cba7f1c4f7269bb12d6c628784fb742e66ed315754dfe38b5984e94d372537f655cb3ea4767c878cbd2d783ee662
        )
        self.x = 0x6b2cd935d0192d54e2c942b574c80102c8f8ef67  # 私钥

    def generate_signature(self, M: str) -> Signature:
        q = self.pub_key.q
        k = random.randint(1, q - 1)
        HM = int(self.SHA1(M), 16)
        r = pow(self.pub_key.g, k, self.pub_key.p) % q
        k_reverse = self.mod_inverse(k, q)
        s = (self.x * r + HM) * k_reverse % q
        return Signature(M=M, r=r, s=s)

    def verify_signature(self, M: str, signature: Signature, pub_key: PubKey) -> bool:
        HM = int(self.SHA1(M), 16)
        w = self.mod_inverse(signature.s, pub_key.q) % pub_key.q
        u1 = HM * w % pub_key.q
        u2 = signature.r * w % pub_key.q
        gu1 = pow(pub_key.g, u1, pub_key.p)
        yu2 = pow(pub_key.y, u2, pub_key.p)
        v = (gu1 * yu2) % pub_key.p % pub_key.q
        return signature.r == v

    @staticmethod
    def mod_inverse(a: int, m: int) -> int:
        g, x, y = DSS.extended_gcd(a, m)
        return x % m if g == 1 else 0

    @staticmethod
    def extended_gcd(a: int, b: int) -> tuple:
        if a == 0:
            return (b, 0, 1)
        g, y, x = DSS.extended_gcd(b % a, a)
        return (g, x - (b // a) * y, y)

    @staticmethod
    def SHA1(message: str) -> str:
        return hashlib.sha1(message.encode()).hexdigest()


# 包含 AES 的安全通信类
class SecureProtocol(DSS):
    def __init__(self):
        super().__init__()
        self.aes_key = get_random_bytes(16)  # 128-bit AES key

    def encrypt(self, plaintext: str) -> str:
        cipher = AES.new(self.aes_key, AES.MODE_EAX)
        ciphertext, tag = cipher.encrypt_and_digest(plaintext.encode())
        return base64.b64encode(cipher.nonce + tag + ciphertext).decode()

    def decrypt(self, b64_cipher: str) -> str:
        data = base64.b64decode(b64_cipher.encode())
        nonce, tag, ciphertext = data[:16], data[16:32], data[32:]
        cipher = AES.new(self.aes_key, AES.MODE_EAX, nonce=nonce)
        return cipher.decrypt_and_verify(ciphertext, tag).decode()

    def send(self, message: str) -> str:
        sig = self.generate_signature(message)
        content = f"{message}|{sig.r}|{sig.s}"
        return self.encrypt(content)

    def receive(self, b64_cipher: str) -> bool:
        try:
            content = self.decrypt(b64_cipher)
            message, r, s = content.split('|', 2)
            sig = Signature(message, int(r), int(s))
            valid = self.verify_signature(message, sig, self.pub_key)
            print(f"[B] 解密得到消息：{message}")
            print(f"[B] 验证签名结果：{valid}")
            return valid
        except Exception as e:
            print("[B] 解密或验证失败：", str(e))
            return False


def main():
    print("=== 模拟公共 Wi-Fi 安全传输 ===")
    # 客户端A
    client = SecureProtocol()
    # 服务器B
    server = SecureProtocol()
    server.aes_key = client.aes_key  # 模拟密钥预共享

    # A 发送数据
    message = "用户密码是123456"
    print(f"[A] 原始消息：{message}")
    encrypted = client.send(message)

    # B 接收数据
    print("\n[B] 开始接收消息...")
    result = server.receive(encrypted)

    if result:
        print("✅ 安全传输成功")
    else:
        print("❌ 安全验证失败")


if __name__ == "__main__":
    main()
