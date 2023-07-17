'''
    Coleman-Liau index
0.0588 * L - 0.296 * S - 15.8
L = letters/100 words
S = sentences/100 words
'''
ends = ['.', '?', '!']

text = input("Text: ")

# Count letters & sentences
letter_count = 0
sent_count = 0
for c in text:
    if c in ends:
        sent_count += 1
    elif c.isalpha():
        letter_count += 1

# Count words and reference factor
if text[len(text) - 1] == ' ':
    words_count = text.count(' ')
else:
    words_count = text.count(' ') + 1
words_factor = words_count / 100

# Calculate Coleman-Liau index
index = 0.0588 * (letter_count / words_factor) - 0.296 * (sent_count / words_factor) - 15.8

# Determine output and print
if round(index) > 15:
    print("Grade 16+")
elif round(index) < 1:
    print("Before Grade 1")
else:
    print(f"Grade {round(index)}")
