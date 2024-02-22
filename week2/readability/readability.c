#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    float word = 1.0;
    float letter = 0.0;
    float sentence = 0.0;
    float L = 0.0;
    float S = 0.0;
    int grade = 0;
    string text = get_string("Text: ");

    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letter++;
        }

        else if (text[i] == '!' || text[i] == '?' || text[i] == '.')
        {
            sentence++;
        }

        else if (isblank(text[i]))
        {
            word++;
        }
    }

    L = letter / word * 100.0;
    S = sentence / word * 100.0;

    grade = round(0.0588 * L - 0.296 * S - 15.8);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }

    else
    {
        printf("Grade %i\n", grade);
    }
}
