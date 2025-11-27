import struct
import os
# S-box table
SM4_SBOX_TABLE = [
    0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c,
    0x05, 0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86,
    0x06, 0x99, 0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed,
    0xcf, 0xac, 0x62, 0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa,
    0x75, 0x8f, 0x3f, 0xa6, 0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c,
    0x19, 0xe6, 0x85, 0x4f, 0xa8, 0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb,
    0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35, 0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25,
    0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87, 0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52,
    0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e, 0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38,
    0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1, 0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34,
    0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3, 0x1d, 0xf6, 0xe2, 0x2e, 0x82,
    0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f, 0xd5, 0xdb, 0x37, 0x45,
    0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51, 0x8d, 0x1b, 0xaf,
    0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8, 0x0a, 0xc1,
    0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0, 0x89,
    0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84,
    0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39,
    0x48,
]

# 常数FK
SM4_FK = [0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc]

# 固定参数CK
SM4_CK = [
    0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
    0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
    0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
    0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
    0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
    0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
    0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
    0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
]

def pad_data(data: bytes) -> bytes:
    """PKCS#7补齐"""
    pad_len = 16 - (len(data) % 16)
    return data + bytes([pad_len] * pad_len)

def unpad_data(data: bytes) -> bytes:
    """去除PKCS#7补齐"""
    pad_len = data[-1]
    if pad_len < 1 or pad_len > 16:
        return data
    return data[:-pad_len]

# S 盒替代
def sm4_sbox(x):
    return (
        (SM4_SBOX_TABLE[(x >> 24) & 0xff] << 24) |
        (SM4_SBOX_TABLE[(x >> 16) & 0xff] << 16) |
        (SM4_SBOX_TABLE[(x >> 8) & 0xff] << 8) |
        SM4_SBOX_TABLE[x & 0xff]
    )

def rol(x, n):
    """循环左移"""
    n = n % 32
    return ((x << n) | (x >> (32 - n))) & 0xffffffff

def sm4_l(x):
    """修正后的线性变换L"""
    return x ^ rol(x, 2) ^ rol(x, 10) ^ rol(x, 18) ^ rol(x, 24)

def sm4_l_(x):
    """修正后的线性变换L'（用于密钥扩展）"""
    return x ^ rol(x, 13) ^ rol(x, 23)

# T 变换
def sm4_t(x, mode):
    if mode == 0:
        return sm4_l(sm4_sbox(x))
    else:
        return sm4_l_(sm4_sbox(x))

def sm4_encrypt_without_padding(mk, plaintext: bytes) -> bytes:
    """加密时不填充，直接处理16字节块"""
    if len(plaintext) % 16 != 0:
        raise ValueError("明文长度必须是16的倍数")
    ciphertext = b''
    for i in range(0, len(plaintext), 16):
        block = plaintext[i:i+16]
        ciphertext += sm4_encrypt_or_decrypt_block(mk, block, encrypt=True)
    return ciphertext

def key_expansion(mk: bytes):
    if len(mk) != 16:
        raise ValueError("密钥长度必须为16字节")

    k = list(struct.unpack(">4I", mk))
    k[0] ^= SM4_FK[0]
    k[1] ^= SM4_FK[1]
    k[2] ^= SM4_FK[2]
    k[3] ^= SM4_FK[3]

    rk = []
    for i in range(32):
        t = k[i + 1] ^ k[i + 2] ^ k[i + 3] ^ SM4_CK[i]
        rk.append(k[i] ^ sm4_t(t, mode=1))
        k.append(rk[-1])

    return rk

def sm4_encrypt_or_decrypt_block(mk, block: bytes, encrypt=True) -> bytes:
    """加密/解密单个16字节块"""
    rk = key_expansion(mk)
    if not encrypt:
        rk = rk[::-1]

    x = list(struct.unpack(">4I", block))

    for i in range(32):
        x.append(x[i] ^ sm4_t(x[i+1] ^ x[i+2] ^ x[i+3] ^ rk[i], mode = 0 ) & 0xffffffff)

    return struct.pack(">4I", *x[-4:][::-1])

def sm4_encrypt(mk, plaintext: str) -> bytes:
    """完整加密流程"""
    # 将字符串转为字节
    data = plaintext.encode('utf-8')
    # 补齐数据
    padded_data = pad_data(data)

    ciphertext = b''
    # 分块处理
    for i in range(0, len(padded_data), 16):
        block = padded_data[i:i+16]
        ciphertext += sm4_encrypt_or_decrypt_block(mk, block, encrypt=True)
    return ciphertext

def sm4_decrypt(mk, ciphertext: bytes) -> str:
    """完整解密流程"""

    if len(ciphertext) % 16 != 0:
        raise ValueError("密文长度必须是16的倍数")

    plaintext = b''
    # 分块解密
    for i in range(0, len(ciphertext), 16):
        block = ciphertext[i:i+16]
        plaintext += sm4_encrypt_or_decrypt_block(mk, block, encrypt=False)

    # 去除补齐
    unpadded = unpad_data(plaintext)
    return unpadded.decode('utf-8', errors='ignore')


# ==== 新增CBC模式相关函数 ====
def generate_iv():
    """生成随机16字节初始化向量"""
    return os.urandom(16)


def xor_bytes(a, b):
    """字节按位异或"""
    return bytes(x ^ y for x, y in zip(a, b))


# ==== 修改加密/解密函数 ====
def sm4_encrypt_cbc(mk, plaintext: bytes, iv: bytes) -> bytes:
    """CBC模式加密（支持任意字节数据）"""
    if len(iv) != 16:
        raise ValueError("IV必须为16字节")

    padded_data = pad_data(plaintext)  # 自动处理填充
    ciphertext = b''
    prev_block = iv

    for i in range(0, len(padded_data), 16):
        block = padded_data[i:i + 16]
        xored_block = xor_bytes(block, prev_block)
        encrypted_block = sm4_encrypt_or_decrypt_block(mk, xored_block, encrypt=True)
        ciphertext += encrypted_block
        prev_block = encrypted_block

    return ciphertext


def sm4_decrypt_cbc(mk, ciphertext: bytes, iv: bytes) -> bytes:
    """CBC模式解密（返回原始字节）"""
    if len(iv) != 16:
        raise ValueError("IV必须为16字节")
    if len(ciphertext) % 16 != 0:
        raise ValueError("密文长度必须是16的倍数")

    plaintext = b''
    prev_block = iv

    for i in range(0, len(ciphertext), 16):
        block = ciphertext[i:i + 16]
        decrypted_block = sm4_encrypt_or_decrypt_block(mk, block, encrypt=False)
        plaintext_block = xor_bytes(decrypted_block, prev_block)
        plaintext += plaintext_block
        prev_block = block

    return unpad_data(plaintext)  # 自动去除填充


def main():
    # 密钥输入（支持16字节任意字符）
    key_type = input("密钥输入方式：1-十六进制 2-文本 (默认1): ") or "1"
    if key_type == "1":
        key_str = input("请输入16字节密钥（32位十六进制）: ").strip()
        if len(key_str) != 32:
            raise ValueError("十六进制密钥长度必须为32位")
        mk = bytes.fromhex(key_str)
    else:
        key_str = input("请输入文本密钥（将自动补齐/截断到16字节）: ").strip()
        mk = key_str.encode('utf-8')[:16].ljust(16, b'\x00')

    # 模式选择
    mode = input("选择模式：1-ECB 2-CBC (默认2): ") or "2"

    # 明文输入
    input_type = input("明文输入方式：1-十六进制 2-文本 (默认2): ") or "2"
    if input_type == "1":
        plaintext_hex = input("请输入明文（十六进制）: ").strip()
        plaintext = bytes.fromhex(plaintext_hex)
    else:
        plaintext_str = input("请输入文本明文: ")
        plaintext = plaintext_str.encode('utf-8')

    # IV处理
    iv = b'\x00' * 16  # 默认值
    if mode == "2":
        iv_type = input("IV输入方式：1-随机生成 2-十六进制输入 3-文本输入 (默认1): ") or "1"
        if iv_type == "1":
            iv = os.urandom(16)
            print(f"生成的IV（十六进制）: {iv.hex()}")
        elif iv_type == "2":
            iv_hex = input("请输入IV（32位十六进制）: ").strip()
            if len(iv_hex) != 32:
                raise ValueError("十六进制IV必须为32位")
            iv = bytes.fromhex(iv_hex)
        else:
            iv_str = input("请输入IV文本（自动处理到16字节）: ")
            iv = iv_str.encode('utf-8')[:16].ljust(16, b'\x00')

    # 加密处理
    if mode == "1":
        ciphertext = sm4_encrypt_without_padding(mk, pad_data(plaintext))  # ECB需要手动填充
    else:
        ciphertext = sm4_encrypt_cbc(mk, plaintext, iv)

    print("\n加密结果:")
    print(f"HEX格式: {ciphertext.hex()}")
    print(f"Base64格式: {base64.b64encode(ciphertext).decode()}")

    # 解密测试
    if mode == "1":
        decrypted = sm4_encrypt_without_padding(mk, ciphertext)
        decrypted = unpad_data(decrypted)  # ECB需要手动去填充
    else:
        decrypted = sm4_decrypt_cbc(mk, ciphertext, iv)

    print("\n解密结果:")
    try:
        print("文本格式:", decrypted.decode('utf-8'))
    except UnicodeDecodeError:
        print("HEX格式:", decrypted.hex())


if __name__ == "__main__":
    import base64  # 新增base64支持
    main()

# key: 0123456789abcdeffedcba9876543210(HEX)
# plaintext: 0123456789abcdeffedcba9876543210(utf-8)
# ciphertext: c91e45ce29f3776a3728b92ff80948faf6e13046124039fb937c762d074092cc1b4bc37a299f647ac7c46ae40e8521d7(HEX)
# decrypted: 0123456789abcdeffedcba9876543210