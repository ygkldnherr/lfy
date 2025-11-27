'''
周期置换密码
'''

def z_encrypt(m, pi):
    c = ''
    for i in range(0, len(m), len(pi)):
        block = m[i:i + len(pi)]
        encrypted_block = [''] * len(block)
        for j in range(len(block)):
            encrypted_block[j] = block[pi[j] - 1]
        c += ''.join(encrypted_block) 
    return c

def z_decrypt(c, pi):
    m = ''
    pi_inv = [pi.index(i) + 1 for i in range(1, len(pi) + 1)]
    for i in range(0, len(c), len(pi)):
        block = c[i:i + len(pi)]
        decrypted_block = [''] * len(block)
        for j in range(len(block)):
            decrypted_block[j] = block[pi_inv[j] - 1]
        m += ''.join(decrypted_block)
    return m

if __name__ == "__main__":
    m = input("请输入明文：")
    pi = list(map(int, input("请输入置换序列（用空格隔开）：").split()))
    c = z_encrypt(m, pi)
    print(f"密文为：{c}")
    m = z_decrypt(c, pi)
    print(f"解密后的明文为：{m}")
