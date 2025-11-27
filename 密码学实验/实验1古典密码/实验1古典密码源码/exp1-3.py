'''
仿射变换
'''

def affine_encrypt(m, a, b):
    c = ''
    for ch in m:
        if ch.isalpha():
            if ch.islower():
                c += chr((a * (ord(ch) - ord('a')) + b) % 26 + ord('a'))
            else:
                c += chr((a * (ord(ch) - ord('A')) + b) % 26 + ord('A'))
        else:
            c += ch
    return c

def affine_decrypt(c, a, b):
    m = ''
    for i in range(1, 27):
        if (i*a % 26) == 1:
            a_i = i
            break
    for ch in c:
        if ch.isalpha():
            if ch.islower():
                m += chr((a_i * (ord(ch) - ord('a') - b)) % 26 + ord('a'))
            else:
                m += chr((a_i * (ord(ch) - ord('A') - b)) % 26 + ord('A'))
        else:
            m += ch
    return m

if __name__ == "__main__":
    # ifyoucanreadthisthankateahcer
    c = input("请输入密文: ")
    a, b = map(int, input("请输入密钥（用空格隔开）：").split())
    m = affine_decrypt(c, a, b)
    print(f"解密后的明文为：{m}")
    c = affine_encrypt(m, a, b)
    print(f"密文为：{c}")
    