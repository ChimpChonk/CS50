while True:
    userinput  = input("Height: ")
    if userinput .isdigit():
        height = int(userinput)
        if height <= 8 and height >= 1:
            break

for i in range(height):
    space = " " * (height - i - 1)
    block = "#" * (i + 1)
    print(space + block + "  " + block)
