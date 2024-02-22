import re

def main():
    # used https://regex101.com/ to verify regex
    pattern = "^(?:\d{16}|\d{15}|\d{13})$"
    card_number = input("Number: ")

    if (re.search(pattern, card_number)):
        card = type(card_number)
        print(card)
    else:
        print("INVALID")

def check_sum(card_number, length):

    sum = 0
    doubleDigit = False

    for i in range(length - 1, -1, -1):
        digit = int(card_number[i])

        if doubleDigit:
            digit *= 2
            if digit > 9:
                digit -= 9
        sum += digit
        doubleDigit = not doubleDigit
    return sum % 10 == 0

# used https://regex101.com/ to verify regex
def type(card_number):
    length = len(card_number)
    if check_sum(card_number, length):
        if re.match(r"^3[47]\d{13}$", card_number):
            return "AMEX"
        elif re.match(r"^5[1-5]\d{14}$", card_number):
            return "MASTERCARD"
        elif re.match(r"^4\d{12}(\d{3})?$", card_number):
            return "VISA"
        else:
            return "INVALID"
    else:
        return "INVALID"

if __name__ == "__main__":
    main()
