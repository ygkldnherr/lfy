import random, math


# 按位与运算
def And(a, b):
    result = ''
    if len(a) != len(b):
        print('len(a)!=len(b)')
        return False
    for i in range(len(a)):
        if (a[i] == '1') & (b[i] == '1'):
            result += '1'
        else:
            result += '0'
    return result


# 三变量按位与运算
def And3(a, b, c):
    return And(And(a, b), c)


# 按位或运算
def Or(a, b):
    result = ''
    if len(a) != len(b):
        print('len(a)!=len(b)')
        return False
    for i in range(len(a)):
        if (a[i] == '1') | (b[i] == '1'):
            result += '1'
        else:
            result += '0'
    return result


# 三变量按位或运算
def Or3(a, b, c):
    return Or(Or(a, b), c)


# 按位异或
def Xor(a, b):
    result = ''
    if len(a) != len(b):
        print('len(a)!=len(b)')
        return False
    for i in range(len(a)):
        if a[i] == b[i]:
            result += '0'
        else:
            result += '1'
    return result


# 三变量按位异或运算
def Xor3(a, b, c):
    return Xor(Xor(a, b), c)


# 按位非运算
def Not(a):
    result = ''
    for ch in a:
        if ch == '1':
            result = result + '0'
        else:
            result = result + '1'
    return result


# 模2^32算数加
def Mod32(a, b):
    c = (a + b)
    d = c % (2 ** 32)
    ans = str(d)
    return ans


# 循环左移函数
def LeftRotate(text, num):
    text = str(text)
    return (text[num:] + text[:num])


# 布尔函数FFj
def FF(x, y, z, j):
    if ((j >= 0) & (j <= 15)):
        ans = Xor3(x, y, z)
    else:
        ans = Or3(And(x, y), And(x, z), And(y, z))
    return ans


# 布尔函数GGj
def GG(x, y, z, j):
    if ((j >= 0) & (j <= 15)):
        ans = Xor3(x, y, z)
    else:
        ans = Or(And(x, y), And(Not(x), z))
    return ans


# 置换函数
def P(x, mode):
    if mode == 0:
        ans = Xor3(x, LeftRotate(x, 9), LeftRotate(x, 17))
    else:
        ans = Xor3(x, LeftRotate(x, 15), LeftRotate(x, 23))
    return ans


# 填充消息m
def Fill(m):
    # 获得m二进制串
    m_bin = ''
    for ch in m:
        ascii_ch = ord(ch)
        m_bin = m_bin + '0' + bin(ascii_ch)[2:]
    # print(m_bin)
    # 添加1
    length = len(m_bin)
    m_bin = m_bin + '1'

    # 添加k个0
    while len(m_bin) % 512 != 448:
        m_bin += '0'

    # 为l的二进制表示补齐0
    length_bin = bin(length)[2:]
    while len(length_bin) < 64:
        length_bin = '0' + length_bin

    m_bin = m_bin + length_bin
    return m_bin


# 消息扩展
def Expand(Bi):
    w = {}  # Bi划分为132个字
    # 5.3.2 a)
    for i in range(16):
        w[i] = Bi[i * 32:(i + 1) * 32]
    # 5.3.2 b)
    for j in range(16, 68):
        tmp = Xor3(w[j - 16], w[j - 9], LeftRotate(w[j - 3], 15))
        tmp = P(tmp, 1)
        w[j] = Xor3(tmp, LeftRotate(w[j - 13], 7), w[j - 6])
    # 5.3.2 c)
    for j in range(64):
        w[j + 68] = Xor(w[j], w[j + 4])
    for i in w:
        w[i] = ZtoH(w[i])  # 二进制转十六进制
    return w


# 压缩函数
def Compress(w, IV):
    A = IV[0:8]
    B = IV[8:16]
    C = IV[16:24]
    D = IV[24:32]
    E = IV[32:40]
    F = IV[40:48]
    G = IV[48:56]
    H = IV[56:64]
    SS1 = ''
    SS2 = ''
    TT1 = ''
    TT2 = ''

    for j in range(64):
        if int(j) <= 15:
            T = '79cc4519'
        else:
            T = '7a879d8a'

        tmp = int(LeftRotate(HtoB(A), 12), 2) + int(HtoB(E), 2) + int(LeftRotate(HtoB(T), j % 32), 2)
        tmp = Mod32(tmp, 0)
        SS1 = LeftRotate(OtoB(tmp), 7)
        SS2 = Xor(SS1, LeftRotate(HtoB(A), 12))

        tmp = int(FF(HtoB(A), HtoB(B), HtoB(C), j), 2) + int(HtoB(D), 2) + int(SS2, 2) + int(HtoB(w[j + 68]), 2)
        tmp = Mod32(tmp, 0)
        TT1 = int(tmp, 10)

        tmp = int(GG(HtoB(E), HtoB(F), HtoB(G), j), 2) + int(HtoB(H), 2) + int(SS1, 2) + int(HtoB(w[j]), 2)
        tmp = Mod32(tmp, 0)
        TT2 = int(tmp, 10)

        D = C
        C = ZtoH(LeftRotate(HtoB(B), 9))
        B = A
        A = OtoH(TT1)
        H = G
        G = ZtoH(LeftRotate(HtoB(F), 19))
        F = E
        E = ZtoH(P(OtoB(TT2), 0))

    r = A + B + C + D + E + F + G + H
    r = HtoB(r)
    v = HtoB(IV)
    return BtoH(Xor(r, v))


# 迭代
def Iteration(m, w):
    IV = {}
    IV[0] = '7380166f4914b2b9172442d7da8a0600a96f30bc163138aae38dee4db0fb0e4e'
    length = len(m)
    n = length // 512
    b = {}  # 对消息按512bit分组
    for i in range(n):
        b[i] = m[512 * i:512 * (i + 1)]
        w = Expand(b[i])
        IV[i + 1] = Compress(w, IV[i])
    return HtoB(IV[n])


# 2进制转16进制，用于expand函数
def ZtoH(text):
    text = str(text)
    while len(text) < 32:
        text = '0' + text
    text_16 = ''
    for i in range(8):
        tmp = hex(int(text[4 * i:4 * (i + 1)], base=2))[2:]
        text_16 = text_16 + tmp
    return text_16


# 2进制转16进制
def BtoH(text):
    text = str(text)
    while len(text) < 32:
        text = '0' + text
    text_16 = ''
    for i in range(len(text) // 4):
        tmp = hex(int(text[4 * i:4 * (i + 1)], base=2))[2:]
        text_16 = text_16 + tmp
    return text_16


# 16进制转2进制
def HtoB(text):
    text_2 = ''
    text = str(text)
    for ch in text:
        tmp = bin(int(ch, base=16))[2:]
        for i in range(4):
            if len(tmp) % 4 != 0:
                tmp = '0' + tmp
        text_2 = text_2 + tmp
    while len(text_2) < 32:
        text_2 = '0' + text_2
    return text_2


# 10进制转2进制
def OtoB(text):
    text_10 = ''
    text = str(text)
    tmp = bin(int(text, base=10))[2:]
    text_10 = text_10 + tmp
    while len(text_10) < 32:
        text_10 = '0' + text_10
    return text_10


# 10进制转16进制
def OtoH(text):
    text_10 = ''
    text = str(text)
    tmp = hex(int(text, base=10))[2:]
    text_10 = text_10 + tmp
    while len(text_10) < 8:
        text_10 = '0' + text_10
    return text_10


# digest
def digest(c):
    m = Fill(c)
    w = Expand(m)
    b = Iteration(m, w)
    return b

# 扩展欧几里得算法求逆元
def get_gcd(a, b):
    if (b == 0):
        return 1, 0, a
    else:
        x, y, gcd = get_gcd(b, a % b)
        x, y = y, (x - (a // b) * y)
        return x, y, gcd


# 两点加法
def add_point(x1, y1, x2, y2, p):
    if (x1 == 'O' and y1 == 'O'):
        return x2, y2
    elif (x2 == 'O' and y2 == 'O'):
        return x1, y1
    elif (x1 == x2 and y2 == ((-1) * y1) % p):
        x3 = 'O'
        y3 = 'O'
        return x3, y3
    else:
        inv, y, gcd = get_gcd(x2 - x1, p)
        lbd = ((y2 - y1) * inv) % p
        x3 = (lbd ** 2 - x1 - x2) % p
        y3 = (lbd * (x1 - x3) - y1) % p
        return x3, y3


# 倍点算法
def multiply2_point(x1, y1, a, p):
    if (x1 == 'O' and y1 == 'O'):
        return x1, y1
    else:
        inv, y, gcd = get_gcd(2 * y1, p)
        lbd = ((3 * (x1 ** 2) + a) * inv) % p
        x3 = (lbd ** 2 - 2 * x1) % p
        y3 = (lbd * (x1 - x3) - y1) % p
        return x3, y3


# k倍点算法
def multiplyk_point(Px, Py, k, a, p):
    k = bin(k)[2:]
    Qx = 'O'
    Qy = 'O'
    for j in range(len(k)):
        Qx, Qy = multiply2_point(Qx, Qy, a, p)
        if (k[j] == '1'):
            Qx, Qy = add_point(Qx, Qy, Px, Py, p)
    return Qx, Qy


# 验证公钥满足条件
def key_statisfy(n, Px, Py, a, b, p):
    # P不能是无穷远点
    if (Px == 'O' or Py == 'O'):
        return False
    # P必须是Fq中的元素
    if (Px < 0 or Py < 0 or Px > p - 1 or Py > p - 1):
        return False
    # P满足椭圆曲线方程
    left = (Py ** 2) % p
    right = (Px ** 3 + a * Px + b) % p
    if (left != right):
        return False
    # [n]P为无穷远点
    nPx, nPy = multiplyk_point(Px, Py, n, a, p)
    if (nPx != 'O' or nPy != 'O'):
        return False
    return True


# 产生公钥
def gen_keypair(n, Gx, Gy, a, b, p):
    d = random.randint(1, n - 1)
    Px, Py = multiplyk_point(Gx, Gy, d, a, p)
    while (not key_statisfy(n, Px, Py, a, b, p)):
        d = random.randint(1, n - 1)
        Px, Py = multiplyk_point(Gx, Gy, d, a, p)
    return d, Px, Py


# 域元素到比特串的转换
def Fq2bit(alpha, p):
    t = math.ceil(math.log(p, 2))
    M = bin(alpha)[2:]
    while (len(M) % 8 != 0 or len(M) != t):
        M = '0' + M
    # print(M,len(M))
    return M


# 点到比特串转换
def point2bit(xp, yp, p):
    PC = '00000100'  # 选择不压缩模式
    xp_bit = Fq2bit(xp, p)
    yp_bit = Fq2bit(yp, p)
    return PC + xp_bit + yp_bit


# KDF combined with SM3
def KDF(Z, klen):
    v = 256
    ct = 1
    Ha = {}
    for i in range(1, math.ceil(klen / v) + 1):
        Ha[i] = digest(Z + bin(ct)[2:].zfill(32))
        ct += 1
    # klen/v is integer
    index = math.ceil(klen / v)
    Haa = ''
    if (math.ceil(klen / v) == klen / v):
        Haa = Ha[index]
    else:
        Haa = Ha[index][:klen - (v * math.floor(klen / v))]
    K = ''
    for i in range(1, math.ceil(klen / v)):
        K += Ha[i]
    K += Haa
    return K


# 按位异或
def Xor(a, b):
    result = ''
    if len(a) != len(b):
        return False
    for i in range(len(a)):
        if a[i] == b[i]:
            result += '0'
        else:
            result += '1'
    return result


# 比特串转域元素
def bit2Fq(b):
    for i in range(len(b)):
        if (b[i] == '1'):
            b = b[i:]
            break
    return int(b, base=2)


# 消息字符串转比特串
def msg2bit(msg):
    res = ''
    for c in msg:
        a = ord(c)
        res += bin(a)[2:].zfill(8)
    return res


# 比特串转消息字符串
def bit2msg(b):
    res = ''
    for i in range(int(len(b) / 8)):
        cbit = b[i * 8:(i + 1) * 8]
        res += chr(int(cbit, base=2))
    return res


# bit str M
def SM2_encrypt(M, n, Gx, Gy, a, b, p, Px, Py):
    # print('SM2 ENCRYPTION')
    klen = len(M)
    # A1
    k = random.randint(1, n - 1)
    # A2
    x1, y1 = multiplyk_point(Gx, Gy, k, a, p)
    C1 = point2bit(x1, y1, p)
    # A3
    h = math.floor(((math.sqrt(p) + 1) ** 2) / n)
    Sx, Sy = multiplyk_point(Px, Py, h, a, p)
    if (Sx == 'O' or Sy == '0'):
        return False
    # A4
    x2, y2 = multiplyk_point(Px, Py, k, a, p)
    x2_bit = Fq2bit(x2, p)
    y2_bit = Fq2bit(y2, p)
    # A5
    t = KDF(x2_bit + y2_bit, klen)
    if (int(t, base=2) == 0):
        return False
    # A6
    C2 = Xor(M, t)
    # A7
    C3 = digest(x2_bit + M + y2_bit)
    # A8
    C = [C1, C2, C3]
    return C


def SM2_decrypt(C, n, Gx, Gy, a, b, p, d):
    # print('SM2 DECRYPTION')
    C1 = C[0]
    C2 = C[1]
    C3 = C[2]
    klen = len(C2)
    # B1
    PC = C1[:8]  # PC=04
    bit_len = int((len(C1) - 8) / 2)
    x1 = bit2Fq(C1[8:8 + bit_len])
    y1 = bit2Fq(C1[8 + bit_len:])
    left = (y1 ** 2) % p
    right = (x1 ** 3 + a * x1 + b) % p
    if (left != right):
        return False
    # B2
    h = math.floor(((math.sqrt(p) + 1) ** 2) / n)
    Sx, Sy = multiplyk_point(Px, Py, h, a, p)
    if (Sx == 'O' or Sy == '0'):
        return False
    # B3
    x2, y2 = multiplyk_point(x1, y1, d, a, p)
    x2_bit = Fq2bit(x2, p)
    y2_bit = Fq2bit(y2, p)
    # B4
    t = KDF(x2_bit + y2_bit, klen)
    if (int(t, base=2) == 0):
        return False
    # B5
    MM = Xor(C2, t)
    # B6
    u = digest(x2_bit + MM + y2_bit)
    if (u != C3):
        return False
    # B7
    return MM


if __name__ == "__main__":

    # y^2=x^3+ax+b
    # 推荐系统参数
    p = int('FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF', base=16)
    a = int('FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC', base=16)
    b = int('28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93', base=16)
    n = int('FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123', base=16)
    Gx = int('32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7', base=16)
    Gy = int('BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0', base=16)
    # 附录实例参数
    '''
    p=int('8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3',base=16)
    a=int('787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498',base=16)
    b=int('63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A',base=16)
    n=int('8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7',base=16)
    Gx=int('421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D',base=16)
    Gy=int('0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2',base=16)'''

    # 产生公私钥对
    d, Px, Py = gen_keypair(n, Gx, Gy, a, b, p)
    print('d:', hex(d), '\nPx:', hex(Px), '\nPy:', hex(Py))

    # SM2_Encrpytion
    msg = 'encryption standard'
    print('明文为：', msg)
    plain_text = msg2bit(msg)
    cipher_text = SM2_encrypt(plain_text, n, Gx, Gy, a, b, p, Px, Py)
    hexresult = ''
    for c in cipher_text:
        hexresult += hex(int(c, base=2))
    print('加密得到密文：', hexresult)

    # SM2_Decryption
    decrypt_text = SM2_decrypt(cipher_text, n, Gx, Gy, a, b, p, d)
    decrypt_text = bit2msg(decrypt_text)
    print('解密得到明文：', decrypt_text)