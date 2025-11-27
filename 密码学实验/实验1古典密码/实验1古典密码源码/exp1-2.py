'''
移位密码
'''

def shift_decrypt(c):
    for key in range(26):
        m = ''
        for ch in c:
            if ch.isalpha():
                if ch.islower():
                    m += chr((ord(ch) - ord('a') - key) % 26 + ord('a'))
                else:
                    m += chr((ord(ch) - ord('A') - key) % 26 + ord('A'))
            else:
                m += ch
        print(f"key = {key}: {m}")

if __name__ == "__main__":
    c = input("请输入密文：")
    shift_decrypt(c)