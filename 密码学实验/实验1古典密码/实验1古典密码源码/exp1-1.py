'''
实现凯撒密码的加密和解密
'''

def caesar_encrypt(m, key):
    c = ''
    for ch in m:
        if ch.isalpha():
            if ch.islower():
                c += chr((ord(ch) - ord('a') + key) % 26 + ord('a'))
            else:
                c += chr((ord(ch) - ord('A') + key) % 26 + ord('A'))
        else:
            c += ch
    return c

def caesar_decrypt(c, key):
    m = ''
    for ch in c:
        if ch.isalpha():
            if ch.islower():
                m += chr((ord(ch) - ord('a') - key) % 26 + ord('a'))
            else:
                m += chr((ord(ch) - ord('A') - key) % 26 + ord('A'))
        else:
            m += ch
    return m

if __name__ == "__main__":
    m = input("请输入明文：")
    key = int(input("请输入密钥："))
    c = caesar_encrypt(m, key)
    print(f"密文为{c}")
    m = caesar_decrypt(c, key)
    print(f"解密后的明文为{m}")