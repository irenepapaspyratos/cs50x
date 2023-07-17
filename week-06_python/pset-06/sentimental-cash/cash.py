def main():
    coins = [0.25, 0.10, 0.05, 0.01]

    # Get input from user and check if it's float
    change = ' '
    while test(change) == False or float(change) < 0:
        change = input('Change owed: ')

    # Calculate number of coins
    s = calc(float(change), coins, 0, 0)

    print(s)


def test(string):
    try:
        float(string)
        res = True
    except ValueError:
        res = False
    return res


def calc(number, coins, su, index):
    mod = number % coins[index]
    if mod == 0:
        su += (number - mod) / coins[index]
        return int(su)
    else:
        su += (number - mod) / coins[index]
        return calc(round(mod, 2), coins, su, index + 1)


main()
