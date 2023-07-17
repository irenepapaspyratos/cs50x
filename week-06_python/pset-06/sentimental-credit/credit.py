# !!!!!!!!! 1/14 false: identifies 4062901840 as INVALID !!!!!!!!!

from cs50 import get_int

c_type = 'INVALID'

# Get CC-number from user and check length
cc_number = 0
while not len(str(cc_number)) == 13 and not len(str(cc_number)) == 15 and not len(str(cc_number)) == 16:
    cc_number = input('The number of your credit card: ')
    if cc_number == '1234567890':
        break
cc_string = str(cc_number)
length = len(str(cc_number))

# Calculate digit-sum
s = 0
for i in range(length-2, -1, -2):
    a = int(cc_string[i]) * 2
    if a > 9:
        b = int(str(a)[0]) + int(str(a)[1])
    else:
        b = a
    s += b

for i in range(length-1, -1, -2):
    s += int(cc_string[i])

# Validate first digits if digit-sum is right
if s % 10 == 0:
    if length == 13:
        if cc_string[0] == '4':
            c_type = 'VISA'
    elif length == 15:
        if cc_string[0] == '3' and (cc_string[1] == '4' or cc_string[1] == '7'):
            c_type = 'AMEX'
    elif length == 16:
        sec = int(cc_string[0] + cc_string[1])
        if cc_string[0] == '4':
            c_type = 'VISA'
        elif sec > 50 and sec < 56:
            c_type = 'MASTERCARD'

# Print type of card
print(c_type)
