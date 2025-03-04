def case1(a, b):
    if (int)(a / 5) == (int)(b / 5):
        if abs(a - b) < 5:
            return 1
        return 0
    return 0

def case1_return(a):
    if (a + 1) % 5 == 0:
        return a - 4
    return a + 1

def case2(a, b):
    if (a % 5) == (b % 5):
        return 1
    return 0

def case2_return(a):
    if (a + 5) > 24:
        return a - 20
    return a + 5

def case3_return(a, b):
    plus = 5
    if (a < b): plus = -5
    while((int)(a / 5) != (int)(b / 5)):
        b += plus
    return b
