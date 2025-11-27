import secrets

# 简化曲线参数 (仅用于教学演示)
P = 97  # 质数域
A = 2  # 曲线系数a
B = 3  # 曲线系数b
G = (3, 6)  # 生成元
N = 17  # 曲线的阶（G的阶）


def inverse_mod(k, p):
    """模逆元计算（费马小定理，p需为质数）"""
    if k == 0:
        raise ValueError("Division by zero")
    return pow(k, p - 2, p)


def is_on_curve(point):
    """检查点是否在曲线上"""
    if point is None:
        return True  # 无穷远点
    x, y = point
    return (y * y - x * x * x - A * x - B) % P == 0


def point_add(p1, p2):
    """椭圆曲线点加法"""
    if p1 is None:
        return p2
    if p2 is None:
        return p1

    x1, y1 = p1
    x2, y2 = p2

    if x1 == x2:
        if y1 != y2:  # P + (-P) = 无穷远点
            return None
        if y1 == 0:  # 切线垂直
            return None
        # 点加倍
        m = (3 * x1 * x1 + A) * inverse_mod(2 * y1, P)
    else:
        # 普通点加法
        m = (y2 - y1) * inverse_mod(x2 - x1, P)

    x3 = (m * m - x1 - x2) % P
    y3 = (m * (x1 - x3) - y1) % P
    return (x3, y3)


def scalar_mult(k, point):
    """标量乘法"""
    if not is_on_curve(point):
        raise ValueError("点不在曲线上")

    result = None
    current = point

    # 处理负系数
    if k < 0:
        return scalar_mult(-k, (current[0], -current[1] % P))

    while k > 0:
        if k % 2 == 1:
            result = point_add(result, current)
        current = point_add(current, current)
        k = k // 2

    if result is None:
        print(f"警告: 标量乘法返回None (k={k}, point={point})")
    return result


def generate_private_key():
    """生成随机私钥（1 <= key < N）"""
    return secrets.randbelow(N - 1) + 1


def ecdh_demo():
    print(f"曲线参数: y² = x³ + {A}x + {B} mod {P}")
    print(f"生成元G: {G}, 阶N={N}\n")

    # Alice生成密钥对
    a_priv = generate_private_key()
    a_pub = scalar_mult(a_priv, G)
    print(f"Alice私钥: {a_priv}")
    print(f"Alice公钥: {a_pub}")
    assert is_on_curve(a_pub), "Alice公钥不在曲线上!"

    # Bob生成密钥对
    b_priv = generate_private_key()
    b_pub = scalar_mult(b_priv, G)
    print(f"\nBob私钥: {b_priv}")
    print(f"Bob公钥: {b_pub}")
    assert is_on_curve(b_pub), "Bob公钥不在曲线上!"

    # 计算共享密钥
    alice_shared = scalar_mult(a_priv, b_pub)
    bob_shared = scalar_mult(b_priv, a_pub)

    print(f"Alice计算的共享点: {alice_shared}")
    print(f"Bob计算的共享点: {bob_shared}")

    if alice_shared is None or bob_shared is None:
        print("\n错误: 共享密钥计算得到None!")
        print("可能原因:")
        print("- 私钥为0或超过曲线阶")
        print("- 遇到了无穷远点")
        return

    print(f"\n验证一致性: {alice_shared == bob_shared}")

    # 简单密钥派生（仅演示）
    if alice_shared != (None, None):
        shared_key = str(alice_shared[0] + alice_shared[1])[-4:]  # 取最后4位作演示
        print(f"演示用共享密钥: {shared_key}")


if __name__ == "__main__":
    ecdh_demo()