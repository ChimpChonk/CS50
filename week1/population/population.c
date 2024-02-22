#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int startSize = 0;

    do
    {
        startSize = get_int("Start size: ");
    }
    while(startSize < 9);

    // TODO: Prompt for end size
    int endSize = 0;

    do
    {
        endSize = get_int("End size: ");
    }
    while(endSize < startSize);


    // TODO: Calculate number of years until we reach threshold
    int years = 0;
    while(startSize < endSize)
    {
        int gain = 0;
        int loss = 0;

        gain = startSize / 3;
        loss = startSize / 4;
        startSize += gain;
        startSize -= loss;
        years++;

    }

    // TODO: Print number of years
    printf("Years: %i\n", years);
}
