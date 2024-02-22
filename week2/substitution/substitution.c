#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int length = strlen(argv[1]);
    if (length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < length; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must only contain alphabetic characters. \n");
            return 1;
        }

        for (int j = i + 1; j < length; j++)
        {
            if (argv[1][j] == argv[1][i])
            {
                printf("Key must not contrain repeated characters. \n");
                return 1;
            }
        }
    }

    string input = get_string("plaintext: ");
    int input_length = strlen(input);
    char chiper;
    int alpha_indx = 0;
    printf("ciphertext: ");
    for (int i = 0; i < input_length; i++)
    {

        if (isupper(input[i]))
        {
            alpha_indx = input[i] - 'A';
            chiper = argv[1][alpha_indx];
            printf("%c", toupper(chiper));
        }

        else if (islower(input[i]))
        {
            alpha_indx = input[i] - 'a';
            chiper = argv[1][alpha_indx];
            printf("%c", tolower(chiper));
        }

        else
        {
            chiper = input[i];
            printf("%c", chiper);
        }
    }

    printf("\n");
}
