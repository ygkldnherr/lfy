import random
import hashlib
from math import gcd
from typing import NamedTuple
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
import base64


class PubKey(NamedTuple):
    p: int
    q: int
    g: int
    y: int


class Signature(NamedTuple):
    M: str
    r: int
    s: int


class DSS:
    def __init__(self):
        self.pub_key = PubKey(
            p=0xd411a4a0e393f6aab0f08b14d18458665b3e4dbdce2544543fe365cf71c8622412db6e7dd02bbe13d88c58d7263e90236af17ac8a9fe5f249cc81f427fc543f7,
            q=0xb20db0b101df0c6624fc1392ba55f77d577481e5,
            g=0xb3085510021f999049a9e7cd3872ce9958186b5007e7adaf25248b58a3dc4f71781d21f2df89b71747bd54b323bbecc443ec1d3e020dadabbf7822578255c104,
            y=0xb32fbec03175791df08c3f861c81df7de7e0cba7f1c4f7269bb12d6c628784fb742e66ed315754dfe38b5984e94d372537f655cb3ea4767c878cbd2d783ee662
        )
        self.x = 0x6b2cd935d0192d54e2c942b574c80102c8f8ef67  # 私钥
        self.signature = Signature(M="", r=0, s=0)

    def generate_signature(self, M: str) -> Signature:
        self.signature = Signature(M=M, r=0, s=0)
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
        gu1modp = pow(pub_key.g, u1, pub_key.p)
        yu2modp = pow(pub_key.y, u2, pub_key.p)
        v = (gu1modp * yu2modp) % pub_key.p % pub_key.q
        return signature.r == v

    @staticmethod
    def mod_inverse(a: int, m: int) -> int:
        g, x, y = DSS.extended_gcd(a, m)
        if g != 1:
            return 0
        else:
            return x % m

    @staticmethod
    def extended_gcd(a: int, b: int) -> tuple:
        if a == 0:
            return (b, 0, 1)
        else:
            g, y, x = DSS.extended_gcd(b % a, a)
            return (g, x - (b // a) * y, y)

    @staticmethod
    def SHA1(message: str) -> str:
        return hashlib.sha1(message.encode()).hexdigest()


class DSSWithEncryption(DSS):
    def __init__(self):
        super().__init__()
        self.aes_key = get_random_bytes(16)  # AES-128 密钥

    def encrypt_message(self, plaintext: str) -> str:
        cipher = AES.new(self.aes_key, AES.MODE_EAX)
        ciphertext, tag = cipher.encrypt_and_digest(plaintext.encode())
        return base64.b64encode(cipher.nonce + tag + ciphertext).decode()

    def decrypt_message(self, b64_encoded_ciphertext: str) -> str:
        data = base64.b64decode(b64_encoded_ciphertext.encode())
        nonce = data[:16]
        tag = data[16:32]
        ciphertext = data[32:]
        cipher = AES.new(self.aes_key, AES.MODE_EAX, nonce=nonce)
        plaintext = cipher.decrypt_and_verify(ciphertext, tag)
        return plaintext.decode()

    def send_to_B(self, message: str) -> tuple[str, Signature]:
        signature = self.generate_signature(message)
        encrypted_message = self.encrypt_message(message)
        return encrypted_message, signature

    def receive_from_A(self, encrypted_message: str, signature: Signature) -> bool:
        try:
            message = self.decrypt_message(encrypted_message)
        except Exception as e:
            print("解密失败，可能被篡改！")
            return False
        print(f"B 解密后得到的明文: {message}")
        valid = self.verify_signature(message, signature, self.pub_key)
        print(f"B 验证签名结果：{valid}")
        return valid


def main():
    # A 发送信息
    print("=== A端发送 ===")
    dss = DSSWithEncryption()
    message = "你好，我是终端A，以下信息具有法律效力。"
    encrypted_message, signature = dss.send_to_B(message)

    print(f"加密后的消息（base64）:\n{encrypted_message}")
    print(f"签名: r = {hex(signature.r)}\ns = {hex(signature.s)}")

    # B 接收信息
    print("\n=== B端接收 ===")
    dss_receiver = DSSWithEncryption()
    dss_receiver.aes_key = dss.aes_key  # 模拟密钥共享
    result = dss_receiver.receive_from_A(encrypted_message, signature)

    if result:
        print("✅ 安全通信成功，消息可信、完整且发送方可认证。")
    else:
        print("❌ 验证失败，消息可能被篡改或伪造。")


if __name__ == "__main__":
    main()
