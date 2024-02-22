from cs50 import *

word = 1
text = get_string("Text: ")
letter = 0
sentence = 0

for i in range(len(text)):
    if text[i].isalpha():
        letter += 1
    elif text[i] == "!" or text[i] == "?" or text[i] == ".":
        sentence += 1
    elif text[i].isspace():
        word += 1

l = letter / word * 100
s = sentence / word * 100

grade = round(0.0588 * l - 0.296 * s - 15.8)

if grade < 1:
    print("Before Grade 1")
elif grade > 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")
