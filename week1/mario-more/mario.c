#include <cs50.h>
#include <stdio.h>

int get_height(void);
void print_height(int height);

int main(void)
{

    int height = get_height();
    //print the height of the block
    print_height(height);
}

int get_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while(height > 8 || height < 1);

    return height;
}

void print_height(int height)
{
    for(int i = 0; i < height; i++)
    {
        for(int h = height - 1; h > i; h--)
        {
            printf(" ");
        }
        for(int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("  ");
        for(int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}
