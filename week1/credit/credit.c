#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool checkSum(string cardNumber, int length);

int main(void)
{
    long input = 0;
    char cardNumber[256];
    int length = 0;
    char firstNumber;
    char secondNumber;
    do
    {
        input = get_long("Number: ");
    }
    while (input < 1);

    sprintf(cardNumber, "%ld", input);

    length = strlen(cardNumber);
    firstNumber = cardNumber[0];
    secondNumber = cardNumber[1];

    if (checkSum(cardNumber, length) == true)
    {
        if (length == 13 || length == 16)
        {
            if (firstNumber == '4')
            {
                printf("VISA\n");
            }

            else if ((firstNumber == '5' && secondNumber == '1') || (firstNumber == '5' && secondNumber == '2') ||
                     (firstNumber == '5' && secondNumber == '3') || (firstNumber == '5' && secondNumber == '4') ||
                     (firstNumber == '5' && secondNumber == '5'))
            {
                printf("MASTERCARD\n");
            }

            else
            {
                printf("INVALID\n");
            }
        }
        else if (length == 15)
        {
            if ((firstNumber == '3' && secondNumber == '4') || (firstNumber == '3' && secondNumber == '7'))
            {
                printf("AMEX\n");
            }

            else
            {
                printf("INVALID\n");
            }
        }

        else
        {
            printf("INVALID\n");
        }
    }

    else
    {
        printf("INVALID\n");
    }
}

bool checkSum(string cardNumber, int length)
{
    int sum = 0;
    bool doubleDigit = false;

    for (int i = length - 1; i >= 0; i--)
    {
        int digit = cardNumber[i] - '0';

        if (doubleDigit)
        {
            digit *= 2;

            if (digit > 9)
            {
                digit -= 9;
            }
        }

        sum += digit;
        doubleDigit = !doubleDigit;
    }

    return (sum % 10 == 0);
}
