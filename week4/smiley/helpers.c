#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            if (image[i][j].rgbtRed == 0x00 && image[i][j].rgbtGreen == 0x00 && image[i][j].rgbtBlue == 0x00)
            {
                image[i][j].rgbtRed = 0xFF;
                image[i][j].rgbtGreen = 0xFF;
                image[i][j].rgbtBlue = 0x00;
            }
        }
    }
}
