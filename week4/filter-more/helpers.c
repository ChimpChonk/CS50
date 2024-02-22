#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop for eatch pixel left to right up to down
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            // get average pixel value.
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // change the pixel value to average value
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop trough height
    for (int i = 0, h = height; i < h; i++)
    {
        // Reverse the rows
        int start = 0;
        int end = width - 1;

        // loop trough the coloum
        while (start < end)
        {
            // temp holds the value of the start row pixel
            RGBTRIPLE temp = image[i][start];
            // image start row gets value of the end row
            image[i][start] = image[i][end];
            // image end coloum gets the value of temp which holds the value of the start row
            image[i][end] = temp;

            // move towards the middle
            start++;
            end--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_image[height][width];

    // create a copy of orginal image
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            copy_image[i][j] = image[i][j];
        }
    }

    // loop trough the pixels
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            int red = 0;
            int green = 0;
            int blue = 0;
            double count = 0.0;

            // loop trought each pixel in the 3x3 neighborhood
            for (int row = i - 1; row <= i + 1; row++)
            {
                for (int column = j - 1; column <= j + 1; column++)
                {
                    // check if the surrounding pixels is within bounds
                    if (row >= 0 && row < height && column >= 0 && column < width)
                    {
                        red += copy_image[row][column].rgbtRed;
                        green += copy_image[row][column].rgbtGreen;
                        blue += copy_image[row][column].rgbtBlue;
                        count++;
                    }
                }
            }
            // average
            image[i][j].rgbtRed = round(red / count);
            image[i][j].rgbtGreen = round(green / count);
            image[i][j].rgbtBlue = round(blue / count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_image[height][width];

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // create a copy of orginal image
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {
            copy_image[i][j] = image[i][j];
        }
    }

    // loop trough the pixels
    for (int i = 0, h = height; i < h; i++)
    {
        for (int j = 0, w = width; j < w; j++)
        {

            double gx_red = 0;
            double gy_red = 0;

            double gx_green = 0;
            double gy_green = 0;

            double gx_blue = 0;
            double gy_blue = 0;

            // loop trought each pixel in the 3x3 neighborhood
            for (int row = -1; row <= 1; row++)
            {
                for (int column = -1; column <= 1; column++)
                {
                    int current_row = i + row;
                    int current_col = j + column;
                    // check if the surrounding pixels is within bounds
                    if (current_row >= 0 && current_row < height && current_col >= 0 && current_col < width)
                    {
                        // Apply Sobel operator for each color
                        gx_red += copy_image[current_row][current_col].rgbtRed * gx[row + 1][column + 1];
                        gy_red += copy_image[current_row][current_col].rgbtRed * gy[row + 1][column + 1];

                        gx_green += copy_image[current_row][current_col].rgbtGreen * gx[row + 1][column + 1];
                        gy_green += copy_image[current_row][current_col].rgbtGreen * gy[row + 1][column + 1];

                        gx_blue += copy_image[current_row][current_col].rgbtBlue * gx[row + 1][column + 1];
                        gy_blue += copy_image[current_row][current_col].rgbtBlue * gy[row + 1][column + 1];
                    }
                }
            }

            // calc magnitude of the gradient for each color
            int mag_red = round(sqrt((gx_red * gx_red) + (gy_red * gy_red)));
            int mag_green = round(sqrt((gx_green * gx_green) + (gy_green * gy_green)));
            int mag_blue = round(sqrt((gx_blue * gx_blue) + (gy_blue * gy_blue)));

            // if value over 255 set 255
            mag_red = (mag_red > 255) ? 255 : mag_red;
            mag_green = (mag_green > 255) ? 255 : mag_green;
            mag_blue = (mag_blue > 255) ? 255 : mag_blue;

            image[i][j].rgbtRed = mag_red;
            image[i][j].rgbtGreen = mag_green;
            image[i][j].rgbtBlue = mag_blue;
        }
    }
    return;
}
