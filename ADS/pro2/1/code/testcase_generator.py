import random
import sys

sys.stdout = open('./test_case.txt', mode='w', encoding='utf-8')
随机数总数 = 500    # 不能超过1000， 也不要小于100， 当小于200的时候可能会出现越界问题


def 输入你的参数():
    N, M = input().split()  # 这是你想要生成测试数据的N和M值
    N = int(N)
    M = int(M)
    return N, M


def 整一份数据(N, M):
    水果 = []
    价格 = []
    tips = []

    for i in range(随机数总数):
        随机数 = abs(int(random.gauss(100, 50)))
        if 随机数 > 1000 or 随机数 == 0:
            随机数 = 100
        水果.append(random.randint(1, 随机数总数-1))
        价格.append(随机数)

    # 给水果这个集合去一下重复
    水果 = set(水果)
    水果 = list(水果)
    random.shuffle(水果)

    for i in range(N):
        tips.append((水果[random.randint(0, len(水果)-1)],
                     水果[random.randint(0, len(水果)-1)]))

    print(N, M)
    for i in range(N):
        print("%03d %03d" % (tips[i][0], tips[i][1]))

    for i in range(M):
        print("%03d %d" % (水果[i], 价格[i]))


if __name__ == "__main__":
    N, M = 输入你的参数()
    整一份数据(N, M)
