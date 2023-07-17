from cs50 import get_int

# Get height from user
h = get_int("Height: ")
while not h > 0 or not h < 9:
    h = get_int("Height: ")

# Print blocks
for i in range(h):
    j = i + 1
    print((' ' * (h-j)) + ('#' * j) + '  ' + ('#' * j))
