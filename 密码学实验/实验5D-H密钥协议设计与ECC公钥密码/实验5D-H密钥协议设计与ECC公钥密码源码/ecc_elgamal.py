# ecc_elgamal.py

def inverse_mod(k, p):
    return pow(k, -1, p)

def point_add(P, Q, a, p):
    if P is None:
        return Q
    if Q is None:
        return P
    x1, y1 = P
    x2, y2 = Q
    if x1 == x2 and y1 != y2:
        return None
    if P == Q:
        s = ((3 * x1 * x1 + a) * inverse_mod(2 * y1, p)) % p
    else:
        s = ((y2 - y1) * inverse_mod(x2 - x1, p)) % p
    x3 = (s * s - x1 - x2) % p
    y3 = (s * (x1 - x3) - y1) % p
    return (x3, y3)

def scalar_mult(k, P, a, p):
    R = None
    while k:
        if k & 1:
            R = point_add(R, P, a, p)
        P = point_add(P, P, a, p)
        k >>= 1
    return R

def elgamal_demo():
    p = 97
    a = 2
    b = 3
    G = (3, 6)

    d = 7  # 私钥
    P = scalar_mult(d, G, a, p)  # 公钥

    M = scalar_mult(2, G, a, p)  # 明文点（假设为 G 的倍数）

    k = 3  # 随机加密因子
    C1 = scalar_mult(k, G, a, p)
    S = scalar_mult(k, P, a, p)
    C2 = point_add(M, S, a, p)

    S2 = scalar_mult(d, C1, a, p)
    neg_S2 = (S2[0], -S2[1] % p)
    M_decrypted = point_add(C2, neg_S2, a, p)

    print("[ElGamal] 明文点 M:", M)
    print("[ElGamal] 密文对 (C1, C2):", C1, C2)
    print("[ElGamal] 解密结果 M':", M_decrypted)
    print("[ElGamal] 解密是否成功:", M == M_decrypted)

if __name__ == "__main__":
    elgamal_demo()
